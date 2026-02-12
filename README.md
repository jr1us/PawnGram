<p align="center">
  <img src="https://github.com/user-attachments/assets/bcbc5788-9770-4adb-a9b0-898a8feedaa7" width="300" style="display:inline-block; margin-right:10px;" />
  <img src="https://github.com/user-attachments/assets/e6f02cf0-8a76-43a4-8590-09ed1f2b724a" width="300" style="display:inline-block;" />
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

# PawnGram — Telegram Bot Library for Pawn Language

**PawnGram** — lightweight library for creating Telegram bots using the Pawn language.

> Designed for simple bots: notifications, messages, ID handling, quick integrations without excessive logic.  
>
> Not suitable for complex, feature-rich bots.  
>
> Documentation:  
>
> [Russian](README.ru.md)

**Example systems:**  
>
> [Account authentication](https://youtu.be/iWmfeV_JrQw)

---

## Installation 

1. Copy all library files into the `pawno/include/` directory.

---

## Integration

**Important:**  
Add `#pragma dynamic 65536` at the beginning of your script to prevent memory errors when working with Telegram API and JSON.

```pawn
#pragma dynamic 65536

#define BOT_TOKEN "" // Get token from @BotFather

#include "PawnGram"

callback OnTelegramMessage(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramMessage] New message! userId -> %s", userId);
    return 1;
}
```

### Usage example:

```pawn
callback OnTelegramMessage(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramMessage] New message! userId -> %s", userId);

    if (GetChatMemberStatus("your id group/chat", userId, message) == -1) // This checks subscription to a group or chat. All verification logic must be inside -> GetChatMemberMessage if you want to check! Unfortunately, only one group is supported.
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

## Example callback for subscription checking (after OnTelegramMessage → GetChatMemberStatus)

```pawn
callback GetChatMemberMessage(const userId[], const username[], const message[], const firstName[], const lastName[], const memberStatus[])
{	
    if (!GetChatMember(memberStatus))
        return SendTelegramMessage(userId, "<b>You are not subscribed or not a member of the group/chat @your_channel</b>", "HTML");

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

## Payment Callback Example

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

### Example callback for handling inline buttons

```pawn
callback OnTelegramInlineKeyBoard(userId[], username[], callbackData[], firstName[], lastName[], callbackId[])
{
    new buffer[256];

    if (strcmp(callbackData, "btn2", true) == 0)
    {
        format(buffer, sizeof buffer, "Button 2 pressed, %s!", firstName);
        SendTelegramMessage(userId, buffer);
        AnswerCallbackQuery(callbackId, "Show alert", true);
    }
    else if (strcmp(callbackData, "btn4", true) == 0)
    {
        format(buffer, sizeof buffer, "You are @%s", username);
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

### PawnGram Function Documentation

### Sending messages

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

### Universal function for sending any content type.
	userId — chat or user ID.
	text — message text (optional).
	parse_mode — formatting mode (markdown, html).
	photoUrl — image URL (when sending photos).
	videoUrl — video URL (when sending videos).
	stickerFileId — sticker file_id.
	voiceUrl — voice message URL.
	videoNoteUrl — video note file_id.
	keyboard — JSON string with inline keyboard.
	Message effect (animated visual effect that plays when the message is received; all available effect IDs are listed in PawnGram.Utils.inc)

### All media parameters are mutually exclusive — send only one per call.

### Sending Payments via Telegram Stars / Other Currencies

```pawn
// For Telegram Stars

new payload[32] = "testInvoice", 
    currency[16] = "XTR", 
    Float:price = 15; // If fractional part >= 0.5, amount rounds up (+1 to integer part), otherwise remains unchanged

SendTelegramInvoice(userId, "Test Invoice", "MoneyBack function - RefundStarPayment", payload, .currency = currency, .price = price);

// For rubles | dollars

new payload[32] = "testInvoice",
    provider_token[64] = "token", // Obtain from payment provider in Telegram (check via @BotFather)
    currency[16] = "RUB", // Or USD
    Float:price = 15.05; // Cost: 1 = 1 ruble | 1 = 1 dollar, fractional values allowed

SendTelegramInvoice(userId, "Test Invoice", "MoneyBack function - RefundStarPayment", payload, .currency = currency, .price = price);

```

### Refunding Telegram Stars (chargeId is the Transaction ID)

```pawn
RefundStarPayment(const userId[], const chargeId[])
```

### Editing Messages

```pawn
EditTelegramMessage(const userId[], messageId, const text[] = "", const parse_mode[] = "" const keyboard[] = "", const photoUrl[] = "")
```

### Deleting Messages

```pawn
DeleteTelegramMessage(const userId[], messageId[])
```

### Convenient Inline Keyboard Builder

```pawn
new button[][][MAX_CALLBACK_SIZE] =
{
    {"Button1", "button_1"},
    {"This is button2", "button_2"}
};

new keyBoardJson[1024];

BuildInlineKeyboard(button, sizeof button, 2, keyBoardJson);
SendTelegramMessage(userId, "Inline buttons test", .keyboard = keyBoardJson);

// Sending colored inlineKeyBoard + Emoji

// Styles:
// danger  - red
// success - green
// primary - blue

// After the style comes the ID of the custom emoji

new button[][][MAX_CALLBACK_SIZE] = {
    {"Delete", "btn_delete", "danger", "5461098492216752942"},
    {"Accept", "btn_accept", "success", "5260416304224936047"},
    {"Buy", "btn_buy", "primary", "5298779458918948862"},
    {"Default", "btn_normal", "", ""}
};

new keyBoardJson[1024];

BuildInlineKeyboard(button, sizeof button, 2, keyBoardJson);
SendTelegramMessage(userId, "Inline buttons test", .keyboard = keyBoardJson);

BuildInlineKeyboard(
    const buttons[][][MAX_CALLBACK_SIZE], // Array of buttons
    buttonCount,                          // Number of buttons
    buttonsPerRow,                        // How many buttons in each row
    output[], len = sizeof(output))       // Output buffer
```

### Getting user information (Does not work within a single thread → after sending the request, it can only be processed inside the callback. There is a verification method using GetChatMemberStatus — check the examples for it.)

```pawn
GetTelegramUserInfo(const userId[])
```

### Getting user status in specific chat/channel (bot must be chat/channel admin | asynchronous — request sent here, result handled only in callback)

```pawn
GetChatMemberStatus(const chatId[], const userId[])
```

### Checking whether a user is a member of a group/chat
## After this check, all bot logic continues inside → GetChatMemberMessage

```pawn
GetChatMemberStatus(const chatId[], const userId[], const message[] = "")
```

Author:
- jr1us (t.me/dcapybarov & vk.com/s.fridom)

For any issues — contact me directly.
