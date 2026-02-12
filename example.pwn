#pragma dynamic 65536

#include 					a_samp
#include 					a_http
#include 					a_actor
#include 					a_npc		
#include 					a_objects
	
#include 					a_players
#include 					a_vehicles
#include 					crashdetect
#include 					float
		
#include 					string
#include                    requests

#if defined callback
	#undef callback
#endif

#define callback%0(%1) forward%0(%1);public%0(%1)

main() { }

#define BOT_TOKEN ""

#include "PawnGram"

callback OnTelegramMessage(const userId[], const username[], const message[], const firstName[], const lastName[])
{
    printf("[PawnGram -> OnTelegramMessage] New message! userId -> %s", userId);

	if (GetChatMemberStatus("your id group/chat", userId, message) == -1) // this method check sub group/chat + logic cmd/message in -> GetChatMemberMessage
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

callback GetChatMemberMessage(const userId[], const username[], const message[], const firstName[], const lastName[], const memberStatus[])
{	
	if(!GetChatMember(memberStatus)) 
		return SendTelegramMessage(userId, "<tg-emoji emoji-id=\"5210952531676504517\">&#10084;</tg-emoji> <b>You are not a member of the channel @your_channel</b>", "HTML");

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

callback OnTelegramSuccessfulPayment(const userId[], const payload[], const currency[], const amount[])
{
    if (!strcmp(payload, "testInvoice")) {
		new buffer[32];

		format(buffer, sizeof buffer, "<b>You paid the bill for</b> <code>%s stars</code>", amount);
		SendTelegramMessage(userId, buffer, "HTML", .message_effect_id = MESSAGE_EFFECT_HEART);
	}
}

callback OnTelegramInlineKeyBoard(userId[], username[], callbackData[], firstName[], lastName[], callbackId[])
{
    new buffer[256];

    if (!strcmp(callbackData, "btn_2", true))
    {
        format(buffer, sizeof buffer, "Button 2 pressed, %s!", firstName);
        SendTelegramMessage(userId, buffer);

        AnswerCallbackQuery(callbackId, "Show alert", true);
    }
	else if (!strcmp(callbackData, "btn_delete", true))
    {
        SendTelegramMessage(userId, "Delete action executed!");
        AnswerCallbackQuery(callbackId, "Deleted!", true);
    }
	else if (!strcmp(callbackData, "btn_accept", true))
    {
        SendTelegramMessage(userId, "Accepted successfully!");
        AnswerCallbackQuery(callbackId);
    }
	else if (!strcmp(callbackData, "btn_buy", true))
    {
        SendTelegramMessage(userId, "Purchase initiated!");
        AnswerCallbackQuery(callbackId, "Processing...", true);
    }
    else {
        AnswerCallbackQuery(callbackId);
    }

    return 1;
}

public OnGameModeInit()
{
	return 1;
}