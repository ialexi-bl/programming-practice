const express = require('express')
const jsonwebtoken = require('jsonwebtoken')
const bodyParser = require('body-parser')
const fetch = require('node-fetch')
const path = require('path')

const { APP_PORT, APP_IP, APP_PATH } = process.env

const indexPath = path.join(path.dirname(APP_PATH), 'index.html')
const SECRET = 'ALSDKAJSDHjao82huBKNADIUQ@h7eijdaNK&@hUDKNAiIQ@H&DUHS'

const app = express()
app.use(bodyParser.json())

app.post('/send-msg', (req, res) => {
  const { peerId, text = '', key } = req.body

  if (!peerId || isNaN(+peerId) || !text || !key) {
    return res.send('Нет ключа, ID переписки или сообщения')
  }

  try {
    const payload = jsonwebtoken.verify(key, SECRET, { algorithms: ['HS256'] })

    if (!payload.admin && payload.peerId != peerId) {
      return res.status(400).send('Нет прав отправлять в эту переписку')
    }

    fetch(
      `https://api.vk.com/method/messages.send?access_token=853b8936b5cce99b49f840ff64fba1bb3ba6de127cfeafb3616ced7720a16096e412363e3dbd2c9aa29ca` +
        `&v=5.92` +
        `&peer_id=${peerId}&message=${text}&random_id=${Math.floor(
          Math.random() * 100000000
        )}`
    )
      .then((res) => res.json())
      .then((json) => {
        if (json.error) return res.status(400).send(json.error)
        res.end()
      })
      .catch(() => {
        return res.status(400).send('Неизвестная ошибка')
      })
  } catch (e) {
    return res.status(400).send('Недействительный ключ')
  }
})
app.get('/key', (req, res) => {
  const { key, peerId } = req.query

  if (!peerId || isNaN(+peerId)) {
    return res.send('Неверный ID переписки')
  }

  try {
    const payload = jsonwebtoken.verify(key, SECRET, { algorithms: ['HS256'] })
    if (!payload.admin) {
      return res.status(400).send('Нет прав создавать ключи')
    }

    res.send(jsonwebtoken.sign({ peerId }, SECRET))
  } catch (e) {
    return res.status(400).send('Недействительный ключ')
  }
})
app.get('/*', (req, res) => {
  res.sendFile(indexPath)
})
app.listen(APP_PORT, APP_IP, () => {})

process.on('SIGTERM', () => {
  console.info('SIGTERM signal received.')
  console.log('Closing http server.')
  app.close(() => {
    console.log('Http server closed.')
    process.exit(0)
  })
})
