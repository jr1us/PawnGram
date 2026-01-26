<p align="center">
  <img width="515" height="208" alt="image5" src="https://github.com/user-attachments/assets/bcbc5788-9770-4adb-a9b0-898a8feedaa7" />
</p>

| <img width="400" height="600" alt="image1" src="https://github.com/user-attachments/assets/4ba73f4f-1d6b-43eb-ae6e-ac68d5216ebf" /> | <img width="564" height="298" alt="image2" src="https://github.com/user-attachments/assets/bd1e4918-0ae6-475b-a1c1-961a1bf0fe2d" /> |
|:--:|:--:|
| <img width="350" height="400" alt="image3" src="https://github.com/user-attachments/assets/845fabbe-c360-4bbe-93f4-99c7cd80275f" /> | <img width="500" height="600" alt="image4" src="https://github.com/user-attachments/assets/a336d814-0524-4939-a1b0-5dabfb7ff27b" /> |
|:--:|:--:|
   
# PawnGram — Library for Telegram Bots in Pawn

**PawnGram** — lightweight library for creating Telegram bots in Pawn language.

> The library is intended for simple bots: notifications, messages, IDs, quick integrations without excessive logic.
>
> Not suitable for complex and multifunctional bots.
>
> Documentation:
>
> [Русский](README.ru.md)



**Example systems:**
>
> [Account authorization](https://youtu.be/iWmfeV_JrQw)

---

## Installation

1. Copy all library files to the `pawno/include/` folder.

---

## Quick Start

**Important:**  
Add `#pragma dynamic 65536` at the beginning of your script to avoid memory errors when working with Telegram API and JSON.

```pawn
#pragma dynamic 65536

#define BOT_TOKEN "" // Get your token from @BotFather

#include "PawnGram"

callback OnTelegramMessage(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramMessage] New message! userId -> %s", userId);
    return 1;
}
```

---

## Usage Example

```pawn
callback OnTelegramMessage(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramMessage] New message! userId -> %s", userId);

    if (!strlen(message))
        return 0;

    new buffer[256];

    if (!strcmp(message, "/start", true))
    {
        format(buffer, sizeof buffer, "*Hello, %s! Your ID*: `%s`", firstName, userId);
        SendTelegramMessage(userId, buffer, "markdown");
    }
	else if (!strcmp(message, "/sticker", true)) {

		new stickers[][256] = {
			"CAACAgIAAxkBAAETNwRo8x6fV8gso63eyvy9pI7RGhD7JQACdiAAAiFmgEvTomWJlLZrmDYE", 
			"CAACAgIAAxkBAAETNwZo8x7EbjvhDcqLic4NAciAy8KwQwACcCMAAi4PGErW6C2PO20QBzYE",
			"CAACAgIAAxkBAAETNwho8x7Lcr5jlo7mVizwsl4b4aDsaAACjh0AAuEKiUv-S9BnpAI53TYE"
		};

		SendTelegramSticker(userId, stickers[random(sizeof stickers)]);
	}

	else if (!strcmp(message, "/custom_emoji", true)) {

		new custom_emoji[][64] = {
			"5334725814040674667", 
			"5377385791456555103",
			"5323520794121222108",
			"5775870512127283512",
			"5442678635909621223"
		};

		format(buffer, sizeof buffer, "<tg-emoji emoji-id=\"%s\">&#128525;</tg-emoji> <b>Отправка кастомного эмодзи / Send custom emoji</b>", custom_emoji[random(sizeof custom_emoji)]);

		SendTelegramMessage(userId, buffer, "HTML");
	}

	else if (!strcmp(message, "/emoji", true)) {

		new emoji[][64] = {
			"&#128147;", 
			"&#129320;",
			"&#128512;",
			"&#128526;",
			"&#128545;"
		};

		format(buffer, sizeof(buffer), 
			"%s <b>Отправка обычного эмодзи / Send default emoji!</b>", emoji[random(sizeof emoji)]
		);
		
		SendTelegramMessage(userId, buffer, "HTML");
	}

	else if (!strcmp(message, "/photo", true)) {
		SendTelegramPhoto(userId, "https://img.joomcdn.net/6ad386a00a79511072954393bd626e903ff3569e_1024_1024.jpeg", "*Photo*", "markdown");
	}

	else if (!strcmp(message, "/note", true)) {
		SendTelegramVideoNote(userId, "DQACAgIAAxkBAAIJ32jzD53WlozJwzyuVwRMiGfzjuMeAAL9dAACX-yYS0xYaHnq4TUBNgQ");
	}

	else if (!strcmp(message, "/video", true)) {
		SendTelegramVideo(userId, "https://static.videezy.com/system/resources/previews/000/000/892/original/zon.mp4", "*Video*", "markdown");
	}
    else if (!strcmp(message, "/buttons", true))
    {
        new utf8Message[512];
        new buttonText1[64];
        new buttonText2[64];
        new buttonText3[64];
        new buttonText4[64];

        ConvertWindows1251ToUTF8(firstName, utf8Message, sizeof(utf8Message));
        ConvertWindows1251ToUTF8("Press 1", buttonText1, sizeof(buttonText1));
        ConvertWindows1251ToUTF8("Press 2", buttonText2, sizeof(buttonText2));
        ConvertWindows1251ToUTF8("Press 3", buttonText3, sizeof(buttonText3));
        ConvertWindows1251ToUTF8("Press 4", buttonText4, sizeof(buttonText4));

        new Node:json = JsonObject(
            "chat_id", JsonString(userId),
            "text", JsonString(utf8Message),
            "reply_markup", JsonObject(
                "inline_keyboard", JsonArray(
                    JsonArray(
                        JsonObject(
                            "text", JsonString(buttonText1),
                            "callback_data", JsonString("btn1")
                        ),
                        JsonObject(
                            "text", JsonString(buttonText2),
                            "callback_data", JsonString("btn2")
                        )
                    ),
                    JsonArray(
                        JsonObject(
                            "text", JsonString(buttonText3),
                            "callback_data", JsonString("btn3")
                        ),
                        JsonObject(
                            "text", JsonString(buttonText4),
                            "callback_data", JsonString("btn4")
                        )
                    )
                )
            )
        );

        SendTelegramMessageWithButton(json);
    }

    return 1;
}
```

---

## Callback Example for Inline Button Handling

```pawn
callback OnTelegramInlineKeyBoard(userId[], username[], callbackData[], firstName[], lastName[], callbackId[])
{
    new buffer[256];

    if (strcmp(callbackData, "btn2", true) == 0)
    {
        format(buffer, sizeof buffer, "Кнопка 2 нажата, %s!", firstName);
        SendTelegramMessage(userId, buffer);

        AnswerCallbackQuery(callbackId, "Show allert", true);
    }

	else if (strcmp(callbackData, "btn4", true) == 0)
    {
        format(buffer, sizeof buffer, "Вы @%s", username);
		
        SendTelegramMessage(userId, buffer);

        AnswerCallbackQuery(callbackId);
    }

    else {
        AnswerCallbackQuery(callbackId);
    }

    return 1;
}
```

> To handle inline buttons, be sure to add the `OnTelegramInlineKeyBoard` callback along with `OnTelegramMessage` to your mode.

---

## PawnGram Function Documentation

### Sending Messages

#### SendTelegramMessage
```pawn
SendTelegramMessage(const userId[], const message[], const parse_mode[] = "")
```
Sends a text message to a user.
- **userId** — user or chat id.
- **message** — message text.
- **parse_mode** — formatting mode (`markdown`, `html`). Optional.

#### SendTelegramSticker
```pawn
SendTelegramSticker(const userId[], const stickerFileId[])
```
Sends a sticker to a user.
- **userId** — user id.
- **stickerFileId** — sticker's file_id.

#### SendTelegramPhoto
```pawn
SendTelegramPhoto(const userId[], const photoURL[], const caption[] = "", const parse_mode[] = "")
```
Sends a photo by link.
- **userId** — chat id.
- **photoURL** — photo URL.
- **caption** — photo caption (optional).
- **parse_mode** — caption formatting (optional).

#### SendTelegramVoice
```pawn
SendTelegramVoice(const chatId[], const voiceURL[])
```
Sends a voice message.
- **chatId** — chat id.
- **voiceURL** — voice file URL.

#### SendTelegramVideo
```pawn
SendTelegramVideo(const userId[], const videoURL[], const caption[] = "", const parse_mode[] = "")
```
Sends a video by link.
- **userId** — chat id.
- **videoURL** — video URL.
- **caption** — video caption (optional).
- **parse_mode** — caption formatting (optional).

#### SendTelegramVideoNote
```pawn
SendTelegramVideoNote(const chatId[], const videoNoteURL[])
```
Sends a video note.
- **chatId** — chat id.
- **videoNoteURL** — video note URL.

#### SendTelegramMessageWithButton
```pawn
SendTelegramMessageWithButton(Node:json)
```
Sends a message with inline keyboard (buttons).
- **json** — JSON object for keyboard markup.

---

### Getting Information

#### GetTelegramUserInfo
```pawn
GetTelegramUserInfo(const userId[])
```
Gets user info (first name, last name, username).
- **userId** — user id.

#### GetTelegramUpdates
```pawn
GetTelegramUpdates()
```
Requests new Telegram updates (handled automatically).

#### GetChatMemberStatus
```pawn
GetChatMemberStatus(const chatId[], const userId[])
```
Gets user status in a chat (admin, member).
- **chatId** — chat id.
- **userId** — user id.

#### GetBotInfo
```pawn
GetBotInfo()
```
Gets bot info (name, username, id). Automatically called at initialization.

---

### Interactive

#### AnswerCallbackQuery
```pawn
AnswerCallbackQuery(callbackId[], text[] = "", bool:showAlert = false)
```
Sends a response to inline button press (can show a notification).

---

## Callbacks

- **OnTelegramMessage** — when receiving a new message/command.
- **OnTelegramResponse** — after sending a message/sticker/photo.
- **OnTelegramUserInfo** — when getting user info.
- **OnTelegramUpdatesJSON** — when processing new updates.
- **OnChatMemberStatus** — when getting user status in chat.
- **OnBotInfo** — when getting bot info.
- **OnTelegramInlineKeyBoard** — when pressing an inline button.
- **OnTelegramMessage** — when receiving a normal message.

---

## Author

Author: jr1us ([t.me/dcapybarov](https://t.me/dcapybarov) & [vk.com/s.fridom](https://vk.com/s.fridom))

For any issues — contact me.

---
