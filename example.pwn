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
		new buttons[][][128] = 
		{
			{"button_1", "btn_1"},
			{"button_2", "btn_2"},
			{"button_3", "btn_3"},
			{"4 Кнопка", "btn_4"}
		};

		new keyboardJson[4096];

		BuildInlineKeyboard(buttons, sizeof buttons, 3, keyboardJson); // 3 is the number of buttons per row (Grid layout:
			1 2 3
			  4);

		SendTelegramMessageWithButton(userId, "&#128520; Три кнопки в ряд", keyboardJson, "HTML");
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
