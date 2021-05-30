#include "MathTokenizer.h"

#include <stdio.h>
#include <stdlib.h>

#include "Math.h"
#include "errorio.h"

#define ALPHABET_LENGTH 26
#define pushToken(token, new) (token = token->next = new)

// These must have the same ID's as corresponding
// ComplexOperatorType enum values
// There aren't supposed to be more than 10 operators with
// same beginning part (can be changed in ComplexOperatorToken declaration)
char complexOperators[][7] = {"gcd", "lcm"};
int complexOperatorsCount = 2;

MathToken *tokenizeMath(char string[], char *error) {
  MathToken *start = getStartToken(), *current = start;

  int i = 0;
  char c;

  while ((c = string[i++])) {
    switch (c) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        if (current->type == TT_NUMBER) {
          // If current token is number, add digit to this number
          NumberToken *ntoken = current->data;
          if (ntoken->isFraction) {
            // To fractional part if decimal point was already encountered
            ntoken->decimal = ntoken->decimal * 10 + c - '0';
          } else {
            // Or to whole part otherwise
            ntoken->whole = ntoken->whole * 10 + c - '0';
          }
        } else {
          // Otherwise create new number token
          pushToken(current, getNumberToken(i, c));
        }
        break;
      }
      case ' ': {
        if (current->type == TT_NUMBER) {
          // If digits follow this space, an empty one
          // prevents them from being concatenated to previous number
          pushToken(current, getEmptyToken(i));
        }
        break;
      }
      case '.': {
        if (current->type != TT_NUMBER) {
          pushToken(current, getNumberToken(i, '0'));
        }
        NumberToken *ntoken = current->data;
        ntoken->isFraction = 1;
        break;
      }
      case '+':
      case '-':
      case '/':
      case '*':
      case '^': {
        pushToken(current, getSimpleOperatorToken(i, c));
        break;
      }
      // case '[':
      // case ']':
      case ',':
      case ';': {
        pushToken(current, getPunctuationToken(i, ','));
        break;
      }
      case '(':
      case ')': {
        pushToken(current, getPunctuationToken(i, c));
        break;
      }
      default: {
        // Rejecting any token that is not a letter
        if (c < 'A' || c > 'z') {
          sprintfError(error, string, i,
                       "TokenizationError: invalid token \"%c\"", c);
          freeTokenList(start);
          return NULL;
        }

        // Chahging to lower case
        if (c < 'a') c += ALPHABET_LENGTH;

        ComplexOperatorToken *cotoken;
        // Trying to append token to operator
        if (current->type == TT_COMPLEX_OPERATOR) {
          cotoken = current->data;

          int *option = cotoken->options;
          // If there are more than one options left or if operator is not
          // complete
          if (option[1] != -1 ||
              complexOperators[option[0]][cotoken->current]) {
            // Check if current symbol is a valid continuation
            while (*option != -1) {
              char expected = complexOperators[*option][cotoken->current];
              if (c == expected) {
                // If yes, filter options that are left and continue to the
                // following symbol
                filterOptions(cotoken->options, c, cotoken->current);
                cotoken->current++;
              }
              ++option;
            }
            // Otherwise error
            sprintfError(error, string, i,
                         "TokenizationError: token \"%c\" doesn't form a known "
                         "operator with preceding ones",
                         c);
            freeTokenList(start);
            return NULL;
          }
        }

        // If no operator to append to or this operator is complete, create new
        // operator
        cotoken = malloc(sizeof(ComplexOperatorToken));
        for (int i = 0; i < 10; i++) cotoken->options[i] = -1;

        // Check that current symbol is a valid beginning for any complex
        // operator
        int position = 0;
        for (int i = 0; i < complexOperatorsCount; i++) {
          if (complexOperators[i][0] == c) {
            cotoken->options[position] = i;
            ++position;
          }
        }

        if (!position) {
          sprintfError(error, string, i,
                       "TokenizationError: no operator begins with \"%c\"", c);
          free(cotoken);
          freeTokenList(start);
          return NULL;
        }

        cotoken->current = 1;
        pushToken(current, getComplexOperatortoken(i, cotoken));
        break;
      }

        // Allow breaking from nested loops
        // breakSwitch:;
    }
  }

  return start;
}

MathToken *getStartToken() {
  MathToken *token = malloc(sizeof(MathToken));
  token->next = NULL;
  token->type = TT_START;
  token->position = 0;
  return token;
}
MathToken *getEmptyToken(int position) {
  MathToken *token = malloc(sizeof(MathToken));
  token->next = NULL;
  token->type = TT_EMPTY;
  token->position = position;
  return token;
}
MathToken *getPunctuationToken(int position, char symbol) {
  MathToken *token = malloc(sizeof(MathToken));
  token->next = NULL;
  token->type = TT_PUNCTUATION;
  token->symbol = symbol;
  token->position = position;
  return token;
}
MathToken *getSimpleOperatorToken(int position, char operator) {
  MathToken *token = malloc(sizeof(MathToken));
  token->next = NULL;
  token->type = TT_SIMPLE_OPERATOR;
  token->symbol = operator;
  token->position = position;
  return token;
}
MathToken *getComplexOperatortoken(int position,
                                   ComplexOperatorToken *cotoken) {
  MathToken *token = malloc(sizeof(MathToken));
  token->next = NULL;
  token->type = TT_COMPLEX_OPERATOR;
  token->data = cotoken;
  token->position = position;
  return token;
}
MathToken *getNumberToken(int position, char symbol) {
  NumberToken *ntoken = malloc(sizeof(NumberToken));
  ntoken->whole = symbol - '0';
  ntoken->isFraction = 0;
  ntoken->decimal = 0;

  MathToken *token = malloc(sizeof(MathToken));
  token->type = TT_NUMBER;
  token->next = NULL;
  token->data = ntoken;
  token->position = position;
  return token;
}

void filterOptions(int *options, char c, int position) {
  int i = 0, *option = options;
  while (*option != -1) {
    if (complexOperators[*option][position] == c) {
      options[i] = *option;
      ++i;
    }
    ++option;
  }
  for (; i < 10; i++) {
    options[i] = -1;
  }
}

void freeTokenList(MathToken *start) {
  MathToken *previous;
  while (start) {
    previous = start;
    start = start->next;

    if (previous->type == TT_NUMBER || previous->type == TT_COMPLEX_OPERATOR) {
      free(previous->data);
    }
    free(previous);
  }
}

void printTokensList(MathToken *start) {
  while (start) {
    switch (start->type) {
      case TT_START:
        printf("[START]\n");
        break;
      case TT_EMPTY:
        printf("[EMPTY TOKEN]\n");
        break;
      case TT_NUMBER: {
        NumberToken *ntoken = start->data;
        printf("[NUMBER] %d.%d (fraction: %d)\n", ntoken->whole,
               ntoken->decimal, ntoken->isFraction);
        break;
      }
      case TT_PUNCTUATION:
        printf("[PUNCTUATION] %c\n", start->symbol);
        break;
      case TT_SIMPLE_OPERATOR:
        printf("[OPERATOR] %c\n", start->symbol);
        break;
      case TT_COMPLEX_OPERATOR: {
        ComplexOperatorToken *cotoken = start->data;
        printf("[COMPLEX_OPERATOR] %s\n",
               complexOperators[cotoken->options[0]]);
        break;
      }
    }
    start = start->next;
  }
}