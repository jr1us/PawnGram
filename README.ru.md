<p align="center">
  <img width="515" height="208" alt="image5" src="https://github.com/user-attachments/assets/bcbc5788-9770-4adb-a9b0-898a8feedaa7" />
</p>

| <img width="400" height="600" alt="image1" src="https://github.com/user-attachments/assets/4ba73f4f-1d6b-43eb-ae6e-ac68d5216ebf" /> | <img width="564" height="298" alt="image2" src="https://github.com/user-attachments/assets/bd1e4918-0ae6-475b-a1c1-961a1bf0fe2d" /> |
|:--:|:--:|
| <img width="350" height="400" alt="image3" src="https://github.com/user-attachments/assets/845fabbe-c360-4bbe-93f4-99c7cd80275f" /> | <img width="500" height="600" alt="image4" src="https://github.com/user-attachments/assets/a336d814-0524-4939-a1b0-5dabfb7ff27b" /> |


# PawnGram — Библиотека для Telegram-ботов на языке Pawn

**PawnGram** — легкая библиотека для создания Telegram-ботов на языке Pawn.

> Библиотека предназначена для простых ботов: уведомления, сообщения, ID, быстрые интеграции без избыточной логики.
>
> Не подходит для сложных и многофункциональных ботов.
>
> Документация:
>
> [English](README.ru.md)

**Пример систем:**
>
> [Авторизация в аккаунт](https://youtu.be/iWmfeV_JrQw)

---

## Установка 

1. Скопируйте все файлы библиотеки в папку `pawno/include/`.

---

## Подключение

**Важно:**  
Добавьте `#pragma dynamic 65536` в начале скрипта, чтобы избежать ошибок памяти при работе с Telegram API и JSON.

```pawn
#pragma dynamic 65536

#define BOT_TOKEN "" // Получите токен у @BotFather

#include "PawnGram"

callback OnTelegramMessage(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramMessage] New message! userId -> %s", userId);
    return 1;
}
```

---

## Пример использования

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

## Пример каллбэка для обработки inline-кнопок

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

> Для обработки inline-кнопок обязательно добавьте каллбек `OnTelegramInlineKeyBoard` вместе с `OnTelegramMessage` в свой мод.

---

## Документация по функциям PawnGram

### Отправка сообщений

#### SendTelegramMessage
```pawn
SendTelegramMessage(const userId[], const message[], const parse_mode[] = "")
```
Отправляет текстовое сообщение пользователю.
- **userId** — id пользователя (или chat_id).
- **message** — текст сообщения.
- **parse_mode** — режим форматирования (`markdown`, `html`). Необязательный.

#### SendTelegramSticker
```pawn
SendTelegramSticker(const userId[], const stickerFileId[])
```
Отправляет стикер пользователю.
- **userId** — id пользователя.
- **stickerFileId** — file_id стикера.

#### SendTelegramPhoto
```pawn
SendTelegramPhoto(const userId[], const photoURL[], const caption[] = "", const parse_mode[] = "")
```
Отправляет фото по ссылке.
- **userId** — id чата.
- **photoURL** — URL фотографии.
- **caption** — подпись к фото (опционально).
- **parse_mode** — форматирование подписи (опционально).

#### SendTelegramVoice
```pawn
SendTelegramVoice(const chatId[], const voiceURL[])
```
Отправляет голосовое сообщение.
- **chatId** — id чата.
- **voiceURL** — URL голосового файла.

#### SendTelegramVideo
```pawn
SendTelegramVideo(const userId[], const videoURL[], const caption[] = "", const parse_mode[] = "")
```
Отправляет видео по ссылке.
- **userId** — id чата.
- **videoURL** — URL видео.
- **caption** — подпись к видео (опционально).
- **parse_mode** — форматирование подписи (опционально).

#### SendTelegramVideoNote
```pawn
SendTelegramVideoNote(const chatId[], const videoNoteURL[])
```
Отправляет видеозаметку.
- **chatId** — id чата.
- **videoNoteURL** — URL видеозаметки.

#### SendTelegramMessageWithButton
```pawn
SendTelegramMessageWithButton(Node:json)
```
Отправляет сообщение с инлайн-клавиатурой (кнопками).
- **json** — JSON-объект для разметки клавиатуры.

---

### Получение информации

#### GetTelegramUserInfo
```pawn
GetTelegramUserInfo(const userId[])
```
Получает информацию о пользователе (имя, фамилия, username).
- **userId** — id пользователя.

#### GetTelegramUpdates
```pawn
GetTelegramUpdates()
```
Запрашивает новые обновления Telegram (обрабатывается автоматически).

#### GetChatMemberStatus
```pawn
GetChatMemberStatus(const chatId[], const userId[])
```
Получает статус пользователя в чате (админ, участник).
- **chatId** — id чата.
- **userId** — id пользователя.

#### GetBotInfo
```pawn
GetBotInfo()
```
Получает информацию о самом боте (имя, username, id). Автоматически вызывается при инициализации.

---

### Интерактив

#### AnswerCallbackQuery
```pawn
AnswerCallbackQuery(callbackId[], text[] = "", bool:showAlert = false)
```
Отправляет ответ на нажатие инлайн-кнопки (можно показать уведомление).

---

## Коллбэки

- **OnTelegramMessage** — при получении нового сообщения/команды.
- **OnTelegramResponse** — после отправки сообщения/стикера/фото.
- **OnTelegramUserInfo** — при получении информации о пользователе.
- **OnTelegramUpdatesJSON** — при обработке новых обновлений.
- **OnChatMemberStatus** — при получении статуса пользователя в чате.
- **OnBotInfo** — при получении информации о боте.
- **OnTelegramInlineKeyBoard** — при нажатии на инлайн-кнопку.
- **OnTelegramMessage** — при получении обычного сообщения.

---

## Автор

Автор: jr1us ([t.me/dcapybarov](https://t.me/dcapybarov) & [vk.com/s.fridom](https://vk.com/s.fridom))

По любым ошибкам — пишите мне.

---
