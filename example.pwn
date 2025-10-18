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

#define callback%0(%1) forward%0(%1);public%0(%1)

main() { }

#define BOT_TOKEN ""

#include "PawnGram"

callback OnTelegramMessage(const userId[], const username[], const message[], const firstName[], const lastName[])
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

	else if (!strcmp(message, "/sticker", true)) {

		new stickers[][256] = {
			"CAACAgIAAxkBAAETNwRo8x6fV8gso63eyvy9pI7RGhD7JQACdiAAAiFmgEvTomWJlLZrmDYE", 
			"CAACAgIAAxkBAAETNwZo8x7EbjvhDcqLic4NAciAy8KwQwACcCMAAi4PGErW6C2PO20QBzYE",
			"CAACAgIAAxkBAAETNwho8x7Lcr5jlo7mVizwsl4b4aDsaAACjh0AAuEKiUv-S9BnpAI53TYE"
		};

		SendTelegramSticker(userId, stickers[random(sizeof stickers)]);
	}

	else if (!strcmp(message, "/photo", true)) {
		SendTelegramPhoto(userId, "https://img.joomcdn.net/6ad386a00a79511072954393bd626e903ff3569e_1024_1024.jpeg", "*Photo* `the album` ", "markdown");
	}

	else if (!strcmp(message, "/note", true)) {
		SendTelegramVideoNote(userId, "DQACAgIAAxkBAAIJ32jzD53WlozJwzyuVwRMiGfzjuMeAAL9dAACX-yYS0xYaHnq4TUBNgQ");
	}

	else if (!strcmp(message, "/video", true)) {
		SendTelegramVideo(userId, "https://static.videezy.com/system/resources/previews/000/000/892/original/zon.mp4", "*Video*", "markdown");
	}

	else if (!strcmp(message, "/buttons", true)) {
		
		new utf8Message[512];
		new buttonText1[64];
		new buttonText2[64];
		new buttonText3[64];
		new buttonText4[64];

		ConvertWindows1251ToUTF8(firstName, utf8Message, sizeof(utf8Message));

		ConvertWindows1251ToUTF8("Нажмите 1", buttonText1, sizeof(buttonText1));
		ConvertWindows1251ToUTF8("Нажмите 2", buttonText2, sizeof(buttonText2));
		ConvertWindows1251ToUTF8("Нажмите 3", buttonText3, sizeof(buttonText3));
		ConvertWindows1251ToUTF8("Нажмите 4", buttonText4, sizeof(buttonText4));

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

	else {
		SendTelegramMessage(userId, message);
	}

    return 1;
}

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

public OnGameModeInit()
{
	return 1;
}