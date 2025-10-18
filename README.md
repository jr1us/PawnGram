<img width="400" height="600" alt="image" src="https://github.com/user-attachments/assets/4ba73f4f-1d6b-43eb-ae6e-ac68d5216ebf" />
<img width="564" height="298" alt="image" src="https://github.com/user-attachments/assets/bd1e4918-0ae6-475b-a1c1-961a1bf0fe2d" />

# PawnGram - Library for Creating Telegram Bots in Pawn Language

**PawnGram** — a lightweight library for creating Telegram bots in Pawn language.

> The library is not designed for creating multifunctional bots, but it is perfect for sending notifications, IDs, or simple messages.

## Installation

1. Copy all library modules into the `pawno/include/` folder.

## Quick Start

Important:
Add #pragma dynamic 65536 at the very top of your script to avoid memory issues when working with Telegram API and JSON.

```pawn
#pragma dynamic 65536

#define BOT_TOKEN "" // Get your token from @BotFather

#include "PawnGram"

callback OnTelegramCommand(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramCommand] New message! userId -> %s", userId);
    return 1;
}
```

## Usage Example

```pawn
callback OnTelegramCommand(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramCommand] New message! userId -> %s", userId);

    if (!strlen(message))
        return 0;

    new buffer[256];

    if (!strcmp(message, "/start", true))
    {
        format(buffer, sizeof buffer, "*Hello, %s! Your ID*: `%s`", firstName, userId);
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

> The library is initialized automatically via a hook in the module.

## Documentation

- [Bot API Telegram](https://core.telegram.org/bots/api)
- PawnGram supports basic methods for sending messages, photos, stickers, and videos.

---

## PawnGram Function Documentation

### SendTelegramMessage
```pawn
SendTelegramMessage(const userId[], const message[], const parse_mode[] = "")
```
Sends a text message to the user.
- **userId** — user id (or chat_id).
- **message** — message text.
- **parse_mode** — formatting mode (`markdown`, `html`). Optional.

---

### SendTelegramSticker
```pawn
SendTelegramSticker(const userId[], const stickerFileId[])
```
Sends a sticker to the user.
- **userId** — user id.
- **stickerFileId** — sticker file_id (can be obtained from Bot API).

---

### SendTelegramPhoto
```pawn
SendTelegramPhoto(const chatId[], const photoURL[])
```
Sends a photo by URL.
- **chatId** — chat id.
- **photoURL** — photo URL.

---

### SendTelegramVoice
```pawn
SendTelegramVoice(const chatId[], const voiceURL[])
```
Sends a voice message.
- **chatId** — chat id.
- **voiceURL** — voice file URL.

---

### SendTelegramVideo
```pawn
SendTelegramVideo(const chatId[], const videoURL[])
```
Sends a video by URL.
- **chatId** — chat id.
- **videoURL** — video URL.

---

### SendTelegramVideoNote
```pawn
SendTelegramVideoNote(const chatId[], const videoNoteURL[])
```
Sends a video note.
- **chatId** — chat id.
- **videoNoteURL** — video note URL.

---

### GetTelegramUserInfo
```pawn
GetTelegramUserInfo(const userId[])
```
Gets user info (first name, last name, username).
- **userId** — user id.

---

### GetTelegramUpdates
```pawn
GetTelegramUpdates()
```
Requests new Telegram updates (handled automatically).

---

### GetChatMemberStatus
```pawn
GetChatMemberStatus(const chatId[], const userId[])
```
Gets the user's status in the chat (e.g., admin, member).
- **chatId** — chat id.
- **userId** — user id.

---

### GetBotInfo
```pawn
GetBotInfo()
```
Gets information about the bot itself (name, username, id). Called during initialization.

---

### Callbacks

- **OnTelegramCommand** — called when a new message/command is received.
- **OnTelegramResponse** — called after sending a message/sticker/photo.
- **OnTelegramUserInfo** — called when user info is received.
- **OnTelegramUpdatesJSON** — called when new updates are processed.
- **OnChatMemberStatus** — called when member status in chat is received.
- **OnBotInfo** — called when bot info is received.

---

Author: jr1us (t.me/dcapybarov & vk.com/s.fridom)

Contact me for any errors.
