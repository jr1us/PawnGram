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

#define BOT_TOKEN "" // Token t.me/BotFather

#include "PawnGram"

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

	else if (!strcmp(message, "/sticker", true)) {
		SendTelegramSticker(userId, "CAACAgIAAxkBAAETNu1o8wcDdEgcShEumGmDg7H43ZusGQACFX0AAopS2EhOLKrl1NiZYjYE");
	}

	else if (!strcmp(message, "/photo", true)) {
		SendTelegramPhoto(userId, "https://media.formula1.com/image/upload/t_16by9Centre/c_lfill,w_3392/q_auto/v1740000000/fom-website/2025/Tech%20Weekly/TECH%20WEEKLY%20V1%20.webp");
	}

	else {
		SendTelegramMessage(userId, message);
	}

    return 1;
}
public OnGameModeInit()
{
	return 1;
}