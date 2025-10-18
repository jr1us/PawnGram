<img width="400" height="600" alt="image" src="https://github.com/user-attachments/assets/4ba73f4f-1d6b-43eb-ae6e-ac68d5216ebf" />
<img width="564" height="298" alt="image" src="https://github.com/user-attachments/assets/bd1e4918-0ae6-475b-a1c1-961a1bf0fe2d" />

# PawnGram — Библиотека для создания Telegram-ботов на языке Pawn

**PawnGram** — легкая библиотека для создания Telegram-ботов на языке Pawn.

> Библиотека не предназначена для создания многофункциональных ботов, но отлично подходит для отправки уведомлений, ID или простых сообщений.

## Установка

1. Скопируйте все модули библиотеки в папку `pawno/include/`.

## Быстрое подключение

Важно:  
Добавьте `#pragma dynamic 65536` в самом начале вашего скрипта, чтобы избежать проблем с памятью при работе с Telegram API и JSON.

```pawn
#pragma dynamic 65536

#define BOT_TOKEN "" // Получить токен можно у @BotFather

#include "PawnGram"

callback OnTelegramCommand(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramCommand] New message! userId -> %s", userId);
    return 1;
}
```

## Пример использования

```pawn
callback OnTelegramCommand(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramCommand] New message! userId -> %s", userId);

    if (!strlen(message))
        return 0;

    new buffer[256];

    if (!strcmp(message, "/start", true))
    {
        format(buffer, sizeof buffer, "*Привет, %s! Твой ID*: `%s`", firstName, userId);
        SendTelegramMessage(userId, buffer, "markdown");
    }
    else if (!strcmp(message, "/sticker", true))
    {
        SendTelegramSticker(userId, "CAACAgIAAxkBAAETNu1o8wcDdEgcShEumGmDg7H43ZusGQACFX0AAopS2EhOLKrl1NiZYjYE");
    }
    else if (!strcmp(message, "/photo", true))
    {
        SendTelegramPhoto(userId, "https://media.formula1.com/image/upload/t_16by9Centre/c_lfill,w_3392/q_auto/v1740000000/fom-website/2025/Tech%20Weekly/TECH%20WEEKLY%20V1%20.webp");
    }
    else if (!strcmp(message, "/note", true))
    {
        SendTelegramVideoNote(userId, "DQACAgIAAxkBAAIJ32jzD53WlozJwzyuVwRMiGfzjuMeAAL9dAACX-yYS0xYaHnq4TUBNgQ");
    }
    else if (!strcmp(message, "/video", true))
    {
        SendTelegramVideo(userId, "https://static.videezy.com/system/resources/previews/000/007/141/original/Express_train_to_lower_manhattan.mp4");
    }
    else
    {
        SendTelegramMessage(userId, message);
    }

    return 1;
}
```

> Библиотека инициализируется автоматически через хук в модуле.

## Документация

- [Bot API Telegram](https://core.telegram.org/bots/api)
- PawnGram поддерживает базовые методы отправки сообщений, фотографий, стикеров и видео.

---

## Документация по функциям PawnGram

### SendTelegramMessage
```pawn
SendTelegramMessage(const userId[], const message[], const parse_mode[] = "")
```
Отправляет текстовое сообщение пользователю.
- **userId** — id пользователя (или chat_id).
- **message** — текст сообщения.
- **parse_mode** — режим форматирования (`markdown`, `html`). Необязательный.

---

### SendTelegramSticker
```pawn
SendTelegramSticker(const userId[], const stickerFileId[])
```
Отправляет стикер пользователю.
- **userId** — id пользователя.
- **stickerFileId** — file_id стикера (можно получить из Bot API).

---

### SendTelegramPhoto
```pawn
SendTelegramPhoto(const chatId[], const photoURL[])
```
Отправляет фото по ссылке.
- **chatId** — id чата.
- **photoURL** — URL фотографии.

---

### SendTelegramVoice
```pawn
SendTelegramVoice(const chatId[], const voiceURL[])
```
Отправляет голосовое сообщение.
- **chatId** — id чата.
- **voiceURL** — URL голосового файла.

---

### SendTelegramVideo
```pawn
SendTelegramVideo(const chatId[], const videoURL[])
```
Отправляет видео по ссылке.
- **chatId** — id чата.
- **videoURL** — URL видео.

---

### SendTelegramVideoNote
```pawn
SendTelegramVideoNote(const chatId[], const videoNoteURL[])
```
Отправляет видеозаметку.
- **chatId** — id чата.
- **videoNoteURL** — URL видеозаметки.

---

### GetTelegramUserInfo
```pawn
GetTelegramUserInfo(const userId[])
```
Получает информацию о пользователе (имя, фамилия, username).
- **userId** — id пользователя.

---

### GetTelegramUpdates
```pawn
GetTelegramUpdates()
```
Запрашивает новые обновления Telegram (обрабатывается автоматически).

---

### GetChatMemberStatus
```pawn
GetChatMemberStatus(const chatId[], const userId[])
```
Получает статус пользователя в чате (например, админ, участник).
- **chatId** — id чата.
- **userId** — id пользователя.

---

### GetBotInfo
```pawn
GetBotInfo()
```
Получает информацию о самом боте (имя, username, id). Вызывается при инициализации.

---

### Коллбэки

- **OnTelegramCommand** — вызывается при получении нового сообщения/команды.
- **OnTelegramResponse** — вызывается после отправки сообщения/стикера/фото.
- **OnTelegramUserInfo** — вызывается при получении информации о пользователе.
- **OnTelegramUpdatesJSON** — вызывается при обработке новых обновлений.
- **OnChatMemberStatus** — вызывается при получении статуса пользователя в чате.
- **OnBotInfo** — вызывается при получении информации о боте.

---

Автор: jr1us (t.me/dcapybarov & vk.com/s.fridom)

По любым ошибкам — пишите мне.
