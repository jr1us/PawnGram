<p align="center">
  <img width="515" height="208" alt="PawnGram Logo" src="https://github.com/user-attachments/assets/bcbc5788-9770-4adb-a9b0-898a8feedaa7" />
</p>

---

<table>
  <tr>
    <td valign="top"><img width="528" height="164" alt="Feature 1" src="https://github.com/user-attachments/assets/ab291efe-46ec-4b46-bd03-eb49f7b0b0cb" /></td>
    <td valign="top"><img width="518" height="201" alt="Feature 2" src="https://github.com/user-attachments/assets/6a5a1c2f-e5e9-4fd0-a16e-214b1258fae6" /></td>
    <td valign="top"><img width="514" height="346" alt="Feature 3" src="https://github.com/user-attachments/assets/d412b91f-8737-453b-b2d7-033a3a2746e8" /></td>
  </tr>
</table>

---

<table>
  <tr>
    <td align="center"><img width="400" height="600" alt="Example 1" src="https://github.com/user-attachments/assets/4ba73f4f-1d6b-43eb-ae6e-ac68d5216ebf" /></td>
    <td align="center"><img width="564" height="298" alt="Example 2" src="https://github.com/user-attachments/assets/bd1e4918-0ae6-475b-a1c1-961a1bf0fe2d" /></td>
  </tr>
</table>

---

<table>
  <tr>
    <td align="center"><img width="350" height="400" alt="Example 3" src="https://github.com/user-attachments/assets/845fabbe-c360-4bbe-93f4-99c7cd80275f" /></td>
    <td align="center"><img width="500" height="600" alt="Example 4" src="https://github.com/user-attachments/assets/a336d814-0524-4939-a1b0-5dabfb7ff27b" /></td>
  </tr>
</table>



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

	if (GetChatMemberStatus("your id group/chat", userId, message) == -1) // Это проверка на подписку на группу или чат, вся логика должна быть в -> GetChatMemberMessage если хотите проверить ! Доступна только одна группа к сожалению
		return SendTelegramMessage(userId, "Please try again later");

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
			"CAACAgIAAxkBAAETNwZo8x7EbjvhDcqLic4NAciAy8KwQwACcCMAAi4PGErW6C2PO30QBzYE",
			"CAACAgIAAxkBAAETNwho8x7Lcr5jlo7mVizwsl4b4aDsaAACjh0AAuEKiUv-S9BnpAI53TYE"
		};

		SendTelegramMessage(userId, "", .stickerFileId = stickers[random(sizeof stickers)]);
	}
	else if (!strcmp(message, "/buttons", true))
	{
		new buttons[][2][128] = {
			{"Button 1", "btn_1"},
			{"Button 2", "btn_2"},
			{"Button 3", "btn_3"},
			{"Button 4", "btn_4"}
		};

		new keyboardJson[2096];
		BuildInlineKeyboard(buttons, sizeof buttons, 3, keyboardJson);

		SendTelegramMessage(userId, "&#128520; Three buttons in a row", "HTML", .keyboard = keyboardJson);
	}
	else if (!strcmp(message, "/styled_buttons", true))
	{
		new buttons[][4][128] = {
			{"Delete", "btn_delete", "danger", ""},
			{"Accept", "btn_accept", "success", ""},
			{"Buy", "btn_buy", "primary", "5389049616962453932"},
			{"Normal", "btn_normal", "", ""}
		};

		new keyboardJson[2096];
		BuildInlineKeyboard(buttons, sizeof buttons, 2, keyboardJson);

		SendTelegramMessage(userId, "&#127912; Styled buttons with colors and emoji", "HTML", .keyboard = keyboardJson);
	}

    return 1;
}
```

## Пример каллбэка проверки на подписку (после того как отправили OnTelegramMessage -> GetChatMemberStatus)

```pawn
callback GetChatMemberMessage(const userId[], const username[], const message[], const firstName[], const lastName[], const memberStatus[])
{	
	if(!GetChatMember(memberStatus)) 
		return SendTelegramMessage(userId, "<b>Вы не подписаны или не участник группы/чата @your_channel</b>", "HTML");

	new buffer[256];
	
	if (!strcmp(message, "/invoice", true))
    {
		new payload[32] = "testInvoice", currency[16] = "XTR", Float:price = 15;

        SendTelegramInvoice(userId, "Test Invoice", "MoneyBack function - RefundStarPayment", payload, .currency = currency, .price = price);
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

		format(buffer, sizeof buffer, "<tg-emoji emoji-id=\"%s\">&#128525;</tg-emoji> <b>Send custom emoji</b>", custom_emoji[random(sizeof custom_emoji)]);
		SendTelegramMessage(userId, buffer, "HTML");
	}
	else if (!strcmp(message, "/photo", true))
	{
		SendTelegramMessage(userId, "*Photo*", "markdown", .photoUrl = "https://img.joomcdn.net/6ad386a00a79511072954393bd626e903ff3569e_1024_1024.jpeg");
	}
	else if (!strcmp(message, "/video", true))
	{
		SendTelegramMessage(userId, "*Video*", "markdown", .videoUrl = "https://static.videezy.com/system/resources/previews/000/000/892/original/zon.mp4");
	}
	
	return 1;
}
```

## Пример каллбэка для обработки платежей

```pawn
callback OnTelegramSuccessfulPayment(const userId[], const payload[], const currency[], const amount[])
{
    if (!strcmp(payload, "testInvoice")) {
		new buffer[32];

		format(buffer, sizeof buffer, "<b>You paid the bill for</b> <code>%s stars</code>", amount);
		SendTelegramMessage(userId, buffer, "HTML", .message_effect_id = MESSAGE_EFFECT_HEART);
	}
}
```

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
    const keyboard[] = "",
	const message_effect_id[] = ""
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
	message_effect_id — Эффект при отправке сообщения ( рядом с времен эмодзи, посмотреть все эмодзи можно в PawnGram.Utils.inc )
	
### Все медиа-параметры взаимоисключающие — передавайте только один за раз.

### Отправка оплаты с помощью Telegram Stars / Другой валюты
```pawn
// Для Telegram Stars

new payload[32] = "testInvoice", 
    currency[16] = "XTR", 
	Float:price = 15; // Если остаток >= 0.5, сумма округляется вверх (+1 к целой части), иначе сумма не изменяется

SendTelegramInvoice(userId, "Test Invoice", "MoneyBack function - RefundStarPayment", payload, .currency = currency, .price = price);

// Для рублей | долларов

new payload[32] = "testInvoice",
    provider_token[64] = "token", // Получить можно у платежной системы в телеграме ( узнать можно в @BotFather )
    currency[16] = "RUB", // Либо USD, 
	Float:price = 15.05; // Стоимость 1 = 1 рубль | 1 = 1 доллар, можно отправлять с остатком

SendTelegramInvoice(userId, "Test Invoice", "MoneyBack function - RefundStarPayment", payload, .currency = currency, .price = price);

```

### Возврат Telegram Stars (chargeId это ID Транзакции)
```pawn
RefundStarPayment(const userId[], const chargeId[])
```

### Редактирование сообщения
```pawn
EditTelegramMessage(const userId[], messageId, const text[] = "", const parse_mode[] = "" const keyboard[] = "", const photoUrl[] = "")
```

### Удаление сообщений
```pawn
DeleteTelegramMessage(const userId[], messageId[])
```

### Удобная сборка Inline Кнопок

```pawn
new button[][][MAX_CALLBACK_SIZE] =
{
	{"Кнопка1", "button_1"},
	{"Это кнопка2", "button_2}
};

new keyBoardJson[1024];

BuildInlineKeyboard(button, sizeof button, 2, keyBoardJson);
SendTelegramMessage(userId, "Тест Inline кнопок", .keyboard = keyBoardJson);

// Отправка цветных inlineKeyBoard + Эмодзи

// Стили:
// danger - красный
// success - зеленый
// primary - синий

// После идет Id кастомных эмодзи

new button[][][MAX_CALLBACK_SIZE] = {
	{"Удалить", "btn_delete", "danger", "5461098492216752942"},
	{"Принять", "btn_accept", "success", "5260416304224936047"},
	{"Купить", "btn_buy", "primary", "5298779458918948862"},
	{"Обычная", "btn_normal", "", ""}
};

new keyBoardJson[1024];

BuildInlineKeyboard(button, sizeof button, 2, keyBoardJson);
SendTelegramMessage(userId, "Тест Inline кнопок", .keyboard = keyBoardJson);

BuildInlineKeyboard(
	const buttons[][][MAX_CALLBACK_SIZE], // Массив с кнопками
	buttonCount, // Количество кнопок
	buttonsPerRow, // Сколько кнопок будет в одном ряду
	output[], len = sizeof(output))
```

### Получение информации об пользователи ( не работает в одном потоке -> отправили запрос, обработать можно только в каллбеке | Есть метод проверки с GetChatMemberStatus изучите примеры с ним)
```pawn
GetTelegramUserInfo(const userId[])
```

### Получение информации об пользователи в конкретном чате / канале ( Бот должен быть админом чата/канала | не работает в одном потоке -> отправили запрос, обработать можно только в каллбеке )
```pawn
GetChatMemberStatus(const chatId[], const userId[])
```

### Проверка на то что состоит в группе/чате
## Использование логики бота переходит -> GetChatMemberMessage

```pawn
GetChatMemberStatus(const chatId[], const userId[], const message[] = "")
```

### Автор:
- jr1us (t.me/dcapybarov & vk.com/s.fridom)
	
### По всем проблемам — пишите мне.
