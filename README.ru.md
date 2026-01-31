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

## Пример использования:

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
    else if (!strcmp(message, "/sticker", true))
    {
        new stickers[][256] = {
            "CAACAgIAAxkBAAETNwRo8x6fV8gso63eyvy9pI7RGhD7JQACdiAAAiFmgEvTomWJlLZrmDYE",
            "CAACAgIAAxkBAAETNwZo8x7EbjvhDcqLic4NAciAy8KwQwACcCMAAi4PGErW6C2PO20QBzYE",
            "CAACAgIAAxkBAAETNwho8x7Lcr5jlo7mVizwsl4b4aDsaAACjh0AAuEKiUv-S9BnpAI53TYE"
        };

        SendTelegramMessage(userId, "", .stickerFileId = stickers[random(sizeof stickers)]);
    }
    else if (!strcmp(message, "/custom_emoji", true))
    {
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
    else if (!strcmp(message, "/emoji", true))
    {
        new emoji[][64] = {
            "&#128147;",
            "&#129320;",
            "&#128512;",
            "&#128526;",
            "&#128545;"
        };

        format(buffer, sizeof(buffer), "%s <b>Отправка обычного эмодзи / Send default emoji!</b>", emoji[random(sizeof emoji)]);
        SendTelegramMessage(userId, buffer, "HTML");
    }
    else if (!strcmp(message, "/photo", true))
    {
        SendTelegramMessage(userId, "*Photo*", "markdown", .photoUrl = "https://img.joomcdn.net/6ad386a00a79511072954393bd626e903ff3569e_1024_1024.jpeg");
    }
    else if (!strcmp(message, "/note", true))
    {
        SendTelegramMessage(userId, "", .videoNoteUrl = "DQACAgIAAxkBAAIJ32jzD53WlozJwzyuVwRMiGfzjuMeAAL9dAACX-yYS0xYaHnq4TUBNgQ");
    }
    else if (!strcmp(message, "/video", true))
    {
        SendTelegramMessage(userId, "*Video*", "markdown", .videoUrl = "https://static.videezy.com/system/resources/previews/000/000/892/original/zon.mp4");
    }
    else if (!strcmp(message, "/buttons", true))
    {
        new buttons[][][128] = {
            {"button_1", "btn_1"},
            {"button_2", "btn_2"},
            {"button_3", "btn_3"},
            {"4 Кнопка", "btn_4"}
        };

        new keyboardJson[4096];
        BuildInlineKeyboard(buttons, sizeof buttons, 3, keyboardJson); // 3 это количество кнопок в ряд (Cетка будет такая:
        // 1 2 3
        //   4);
        SendTelegramMessage(userId, "&#128520; Три кнопки в ряд", "HTML", .keyboard = keyboardJson);
    }

    return 1;
}
```

##Пример каллбэка для обработки inline-кнопок

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
    else
    {
        AnswerCallbackQuery(callbackId);
    }

    return 1;
}
```

### Документация по функциям PawnGram

## Отправка сообщений

```pawn 
SendTelegramMessage(
    const userId[],
    const text[] = "",
    const parse_mode[] = "",
    const photoUrl[] = "",
    const videoUrl[] = "",
    const stickerFileId[] = "",
    const voiceUrl[] = "",
    const videoNoteUrl[] = "",
    const keyboard[] = ""
)
```

### Универсальная функция для отправки любого типа контента.
	userId — ID чата или пользователя.
	text — текст сообщения (опционально).
	parse_mode — режим разметки (markdown, html).
	photoUrl — URL изображения (если отправляете фото).
	videoUrl — URL видео (если отправляете видео).
	stickerFileId — file_id стикера.
	voiceUrl — URL голосового сообщения.
	videoNoteUrl — file_id видеозаметки.
	keyboard — JSON-строка с inline-клавиатурой.
	
### Все медиа-параметры взаимоисключающие — передавайте только один за раз.


### Получение информации об пользователи ( не работает в одном потоке -> отправили запрос, обработать можно только в каллбеке )
```pawn
GetTelegramUserInfo(const userId[])
```

### Получение информации об пользователи в конкретном чате / канале ( Бот должен быть админом чата/канала | не работает в одном потоке -> отправили запрос, обработать можно только в каллбеке )
```pawn
GetChatMemberStatus(const chatId[], const userId[])
```

### Автор:
- jr1us (t.me/dcapybarov & vk.com/s.fridom)
	
### По всем проблемам — пишите мне.
