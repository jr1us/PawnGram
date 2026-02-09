/*   
*    *___________________________*
*    * github.com/jr1us.com      *
*    * (c) 2026                  *
*    *                           *
*    * Telegram bots in Pawn.    *
*    *___________________________*
*/

#if !defined callback
    #define callback%0(%1) forward%0(%1);public%0(%1)
#endif

#define BOT_TOKEN ""

#if !defined BOT_TOKEN
    #error "[PawnGram] Please create #define BOT_TOKEN "TokenBotFather" "
    #define BOT_TOKEN ""
#endif

#define API_TELEGRAM_URL "https://api.telegram.org"

#if !defined PAWN_GRAM_DEFINED
    #define PAWN_GRAM_DEFINED
#endif

#if !defined DEBUG_MODE_TELEGRAM
    #define DEBUG_MODE_TELEGRAM 0
#endif

#if !defined SHOW_ERROR_TELEGRAM_API
    #define SHOW_ERROR_TELEGRAM_API 1 
#endif

#define SHOW_ERROR_LANGUAGE "RU" // RU | EN

#include "PawnGram.Utils"

static enum {
    GET_UPDATE_MESSAGE = 1,
    GET_UPDATE_INLINE_KEYBOARD = 2,
    GET_UPDATE_SUCCESSFUL_PAYMENTS = 3,
    GET_UPDATE_ANSWER_PAYMENTS = 4
};

const TELEGRAM_TICKRATE_UPDATE = 1000;

const MAX_CALLBACK_SIZE = 128;
const MAX_SIZE_TELEGRAM_NAME = 64;
const MAX_SIZE_USER_NAME = 32;
const MAX_SIZE_USER_ID = 32;

static bool:botInitialized = false;
static RequestsClient:clientTelegram;
static lastUpdateId = 0; 

static const TelegramErrorsEN[][MAX_CALLBACK_SIZE] = {
    "Bad Request: chat not found",
    "Bad Request: message is not modified",
    "Bad Request: message text is empty", // maybe fix: (If using a message with inline buttons, try increasing the array size)
    "Bad Request: message to delete not found",
    "Bad Request: message can't be deleted",
    "Bad Request: message to edit not found",
    "Bad Request: query is too old or invalid",
    "Bad Request: BUTTON_URL_INVALID",
    "Bad Request: BUTTON_DATA_INVALID",
    "Bad Request: can't parse entities",
    "Bad Request: wrong file identifier/HTTP URL specified",
    "Bad Request: failed to get HTTP URL content",
    "Bad Request: wrong type of the web page content",
    "Bad Request: wrong file_id or the file is temporarily unavailable",
    "Bad Request: photo should be uploaded as an InputFile",
    "Bad Request: PEER_ID_INVALID",
    "Forbidden: bot was blocked by the user",
    "Forbidden: bot can't initiate conversation with a user",
    "Forbidden: bot was kicked from the group chat",
    "Forbidden: bot was kicked from the supergroup chat",
    "Forbidden: bot was kicked from the channel chat",
    "Forbidden: user is deactivated",
    "Unauthorized: bot token is invalid",
    "Not Found: method not found",
    "Conflict: terminated by other getUpdates request",
    "Too Many Requests: retry after X seconds",
    "Bad Gateway: wrong response from the Telegram API",
    "Bad Request: group chat was upgraded to a supergroup",
    "Bad Request: CHAT_WRITE_FORBIDDEN",
    "Bad Request: user not in chat",
    "Bad Request: message to forward not found",
    "Bad Request: cannot delete chat history for everyone",
    "Bad Request: file is too big",
    "Bad Request: sticker set not found",
    "Bad Request: emoji statuses are not available",
    "Bad Request: video duration is too long",
    "Bad Request: location not found",
    "Bad Request: invoice payload is invalid",
    "Bad Request: shipping options are invalid",
    "Bad Request: invoice already paid",
    "Bad Request: invoice not found",
    "Bad Request: callback query already answered",
    "Bad Request: poll is closed",
    "Bad Request: poll is not in correct state",
    "Bad Request: EFFECT_ID_INVALID",
    "Bad Request: PREMIUM_ACCOUNT_REQUIRED (requires Telegram Premium to set a custom bot username)",
    "Bad Request: CURRENCY_TOTAL_AMOUNT_INVALID (prov)",
    "Bad Request: can't parse labeled price: Field \"price\" must be a valid Number",
    "Response: Bad Request: field \"inline_keyboard\" must be of type Array",
    "Forbidden: bots can't send messages to bots"
};

static const TelegramErrorsRU[sizeof TelegramErrorsEN][MAX_CALLBACK_SIZE] = {
    "Чат c пользователем не найден",
    "Сообщение не изменено",
    "Текст сообщения пустой (Если используете сообщение с inline кнопками, попробуйте увеличить размер массива)",
    "Сообщение для удаления не найдено",
    "Сообщение не может быть удалено",
    "Сообщение для редактирования не найдено",
    "Запрос слишком старый или не найден",
    "Неверный URL кнопки",
    "Неверные данные кнопки",
    "Не удалось разобрать форматирование",
    "Неверный идентификатор файла/HTTP URL",
    "Не удалось получить содержимое по HTTP URL",
    "Неверный тип содержимого веб-страницы",
    "Неверный file_id или файл временно недоступен",
    "Фото должно быть загружено как InputFile",
    "Неверный идентификатор получателя",
    "Запрещено: бот заблокирован пользователем",
    "Запрещено: бот не может начать диалог с пользователем",
    "Запрещено: бот исключен из группового чата",
    "Запрещено: бот исключен из супергруппы",
    "Запрещено: бот исключен из канала",
    "Запрещено: пользователь деактивирован",
    "Не авторизован: токен бота неверный",
    "Не найдено: метод не найден",
    "Конфликт: прерван другим запросом getUpdates",
    "Слишком много запросов: повторите через X секунд",
    "Ошибка шлюза: неверный ответ от API Telegram",
    "Неверный запрос: групповой чат обновлён до супергруппы",
    "Неверный запрос: нет прав на запись в чат",
    "Неверный запрос: пользователь не в чате",
    "Неверный запрос: сообщение для пересылки не найдено",
    "Неверный запрос: невозможно удалить историю чата для всех",
    "Неверный запрос: файл слишком большой",
    "Неверный запрос: набор стикеров не найден",
    "Неверный запрос: статусы эмодзи недоступны",
    "Неверный запрос: длительность видео слишком длинная",
    "Неверный запрос: локация не найдена",
    "Неверный запрос: неверная полезная нагрузка счета",
    "Неверный запрос: недействительные варианты доставки",
    "Неверный запрос: счет уже оплачен",
    "Неверный запрос: счет не найден",
    "Неверный запрос: callback-запрос уже обработан",
    "Неверный запрос: опрос закрыт",
    "Неверный запрос: опрос в неверном состоянии",
    "Неверный запрос: неверный message_effect_id для сообщения",
    "Неверный запрос: для использования текущего message_effect_id требуется Telegram Premium (username для бота)",
    "Неверный запрос: общая сумма валюты указана неверно",
    "Неверный запрос: невозможно разобрать цену. Поле \"price\" должно быть допустимым числом. (сумма должна быть без остатка)",
    "Неверный запрос: поле \"inline_keyboard\" должно быть типа Array (массив)",
    "Неверный запрос: боты не могут отправлять сообщения ботам"
};

stock GetBotInit()
    return botInitialized;

stock SendTelegramInvoice(const userId[], const title[], const description[], const payload[], const provider_token[] = "", const currency[] = "XTR", Float:price = 0.0, const photoUrl[] = "")
{
    if (!botInitialized || !IsValidRequestsClient(clientTelegram)) return 0;

    new utf8Title[1024], utf8Desc[1024], endpoint[256];
    ConvertWindows1251ToUTF8(title, utf8Title, sizeof(utf8Title));
    ConvertWindows1251ToUTF8(description, utf8Desc, sizeof(utf8Desc));

    price = (!strcmp(currency, "XTR")) ? floatround(price) : floatround(price) * 100;

    new Node:priceObject = 
        JsonObject("label", 
            JsonString(utf8Title), "amount", 
            JsonFloat(price)
        );

    new Node:pricesArray = JsonArray(priceObject);
    
    new Node:json;
    if(strlen(photoUrl)) {
        json = JsonObject(
            "chat_id", JsonString(userId),
            "title", JsonString(utf8Title),
            "description", JsonString(utf8Desc),
            "payload", JsonString(payload),
            "provider_token", JsonString(provider_token),
            "currency", JsonString(currency),
            "prices", pricesArray,
            "photo_url", JsonString(photoUrl)
        );
    } else {
        json = JsonObject(
            "chat_id", JsonString(userId),
            "title", JsonString(utf8Title),
            "description", JsonString(utf8Desc),
            "payload", JsonString(payload),
            "provider_token", JsonString(provider_token),
            "currency", JsonString(currency),
            "prices", pricesArray
        );
    }

    format(endpoint, sizeof(endpoint), "/bot%s/sendInvoice", BOT_TOKEN);
    new Request:id = RequestJSON(clientTelegram, endpoint, HTTP_METHOD_POST, "OnPostJson", json, .headers = RequestHeaders());
    return _:id;
}

stock RefundStarPayment(const userId[], const chargeId[])
{
    if (!botInitialized || !IsValidRequestsClient(clientTelegram))
        return 0;

    new Node:json, endpoint[256];

    json = JsonObject(
        "user_id", JsonString(userId),
        "telegram_payment_charge_id", JsonString(chargeId)
    );

    format(endpoint, sizeof(endpoint), "/bot%s/refundStarPayment", BOT_TOKEN);

    new Request:id = RequestJSON(
        clientTelegram,
        endpoint,
        HTTP_METHOD_POST,
        "",
        json,
        .headers = RequestHeaders()
    );

    #if DEBUG_MODE_TELEGRAM
        printf("[PawnGram] refundStarPayment -> sent | userId -> %s, chargeId -> %s | id -> %d",
            userId, chargeId, _:id);
    #endif

    return _:id;
}

stock SendTelegramMessage(const userId[], const text[], const parse_mode[] = "", const photoUrl[] = "", const videoUrl[] = "", const stickerFileId[] = "", const voiceUrl[] = "", const videoNoteUrl[] = "", const keyboard[] = "", const message_effect_id[] = "")
{   
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;
    
    if (strlen(keyboard)) {
        new utf8Message[4096], method[64];
        new Node:json, Node:replyMarkup;

        ConvertWindows1251ToUTF8(text, utf8Message, sizeof utf8Message );

        JsonParse(keyboard, replyMarkup);
        
        if (strlen(photoUrl)) {
            method = "/sendPhoto";

            if (strlen(parse_mode) && strlen(message_effect_id)) {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "photo", JsonString(photoUrl),
                    "caption", JsonString(utf8Message),
                    "parse_mode", JsonString(parse_mode),
                    "message_effect_id", JsonString(message_effect_id),
                    "reply_markup", replyMarkup
                );
            }
            else if (strlen(parse_mode)) {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "photo", JsonString(photoUrl),
                    "caption", JsonString(utf8Message),
                    "parse_mode", JsonString(parse_mode),
                    "reply_markup", replyMarkup
                );
            }
            else if (strlen(message_effect_id)) {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "photo", JsonString(photoUrl),
                    "caption", JsonString(utf8Message),
                    "message_effect_id", JsonString(message_effect_id),
                    "reply_markup", replyMarkup
                );
            }
            else {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "photo", JsonString(photoUrl),
                    "caption", JsonString(utf8Message),
                    "reply_markup", replyMarkup
                );
            }
        }
        else if (strlen(videoUrl)) {
            method = "/sendVideo";
            
            if (strlen(parse_mode) && strlen(message_effect_id)) {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "video", JsonString(videoUrl),
                    "caption", JsonString(utf8Message),
                    "parse_mode", JsonString(parse_mode),
                    "message_effect_id", JsonString(message_effect_id),
                    "reply_markup", replyMarkup
                );
            }
            else if (strlen(parse_mode)) {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "video", JsonString(videoUrl),
                    "caption", JsonString(utf8Message),
                    "parse_mode", JsonString(parse_mode),
                    "reply_markup", replyMarkup
                );
            }
            else if (strlen(message_effect_id)) {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "video", JsonString(videoUrl),
                    "caption", JsonString(utf8Message),
                    "message_effect_id", JsonString(message_effect_id),
                    "reply_markup", replyMarkup
                );
            }
            else {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "video", JsonString(videoUrl),
                    "caption", JsonString(utf8Message),
                    "reply_markup", replyMarkup
                );
            }
        }
        else {
            method = "/sendMessage";

            if (strlen(parse_mode) && strlen(message_effect_id)) {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "text", JsonString(utf8Message),
                    "parse_mode", JsonString(parse_mode),
                    "message_effect_id", JsonString(message_effect_id),
                    "reply_markup", replyMarkup
                );
            }
            else if (strlen(parse_mode)) {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "text", JsonString(utf8Message),
                    "parse_mode", JsonString(parse_mode),
                    "reply_markup", replyMarkup
                );
            }
            else if (strlen(message_effect_id)) {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "text", JsonString(utf8Message),
                    "message_effect_id", JsonString(message_effect_id),
                    "reply_markup", replyMarkup
                );
            }
            else {
                json = JsonObject(
                    "chat_id", JsonString(userId),
                    "text", JsonString(utf8Message),
                    "reply_markup", replyMarkup
                );
            }
        }

        new endpoint[256];
        format(endpoint, sizeof(endpoint), "/bot%s%s", BOT_TOKEN, method);

        new Request:id = RequestJSON(clientTelegram, endpoint, HTTP_METHOD_POST, "OnPostJson", json, .headers = RequestHeaders());

        if (!IsValidRequest(id))
            return 0;

        #if DEBUG_MODE_TELEGRAM
            printf("[PawnGram] Request sent id -> %d (OnPostJson)", _:id);
        #endif

        return _:id;
    }

    new utf8Text[2096], encodedText[2096], utf8Media[2096], encodedMedia[2096];
    new utf8Caption[1024], encodedCaption[1024], endpoint[4096];
    
    if (strlen(photoUrl)) {
        ConvertWindows1251ToUTF8(photoUrl, utf8Media, sizeof(utf8Media));
        UrleanCodeExport(utf8Media, encodedMedia, sizeof(encodedMedia));
        format(endpoint, sizeof(endpoint), "/bot%s/sendPhoto?chat_id=%s&photo=%s", BOT_TOKEN, userId, encodedMedia);
    }
    else if (strlen(videoUrl)) {
        ConvertWindows1251ToUTF8(videoUrl, utf8Media, sizeof(utf8Media));
        UrleanCodeExport(utf8Media, encodedMedia, sizeof(encodedMedia));
        format(endpoint, sizeof(endpoint), "/bot%s/sendVideo?chat_id=%s&video=%s", BOT_TOKEN, userId, encodedMedia);
    }
    else if (strlen(stickerFileId)) {
        format(endpoint, sizeof(endpoint), "/bot%s/sendSticker?chat_id=%s&sticker=%s", BOT_TOKEN, userId, stickerFileId);
    }
    else if (strlen(voiceUrl)) {
        ConvertWindows1251ToUTF8(voiceUrl, utf8Media, sizeof(utf8Media));
        UrleanCodeExport(utf8Media, encodedMedia, sizeof(encodedMedia));
        format(endpoint, sizeof(endpoint), "/bot%s/sendVoice?chat_id=%s&voice=%s", BOT_TOKEN, userId, encodedMedia);
    }
    else if (strlen(videoNoteUrl)) {
        ConvertWindows1251ToUTF8(videoNoteUrl, utf8Media, sizeof(utf8Media));
        UrleanCodeExport(utf8Media, encodedMedia, sizeof(encodedMedia));
        format(endpoint, sizeof(endpoint), "/bot%s/sendVideoNote?chat_id=%s&video_note=%s", BOT_TOKEN, userId, encodedMedia);
    }
    else {
        ConvertWindows1251ToUTF8(text, utf8Text, sizeof(utf8Text));
        UrleanCodeExport(utf8Text, encodedText, sizeof(encodedText));
        format(endpoint, sizeof(endpoint), "/bot%s/sendMessage?chat_id=%s&text=%s", BOT_TOKEN, userId, encodedText);
    }

    if ((strlen(photoUrl) || strlen(videoUrl)) && strlen(text)) {
        ConvertWindows1251ToUTF8(text, utf8Caption, sizeof(utf8Caption));
        UrleanCodeExport(utf8Caption, encodedCaption, sizeof(encodedCaption));
        format(endpoint, sizeof(endpoint), "%s&caption=%s", endpoint, encodedCaption);
    }

    if (strlen(parse_mode))
        format(endpoint, sizeof(endpoint), "%s&parse_mode=%s", endpoint, parse_mode);
    
    if (strlen(message_effect_id))
        format(endpoint, sizeof(endpoint), "%s&message_effect_id=%s", endpoint, message_effect_id);

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramResponse");
    
    if (!IsValidRequest(id))
        return 0;

    #if DEBUG_MODE_TELEGRAM
        printf("[PawnGram] Request sent to user %s | id -> %d", userId, _:id);
    #endif

    return _:id;
}

stock EditTelegramMessage(const userId[], messageId, const text[] = "", const parse_mode[] = "" const keyboard[] = "", const photoUrl[] = "")
{
    if (!botInitialized || !IsValidRequestsClient(clientTelegram)) return 0;

    new utf8Message[4096], endpoint[256], Node:json, Node:replyMarkup;
    ConvertWindows1251ToUTF8(text, utf8Message, sizeof(utf8Message));

    if (strlen(keyboard)) {
        new Node:keyboardNode;
        JsonParse(keyboard, keyboardNode);
        replyMarkup = JsonObject("inline_keyboard", keyboardNode);
    }

    if (strlen(photoUrl)) 
    {
        new Node:media = JsonObject(
            "type", JsonString("photo"),
            "media", JsonString(photoUrl),
            "caption", JsonString(utf8Message),
            "parse_mode", JsonString(parse_mode)
        );

        if (strlen(keyboard)) {
            json = JsonObject(
                "chat_id", JsonString(userId),
                "message_id", JsonInt(messageId),
                "media", media,
                "reply_markup", replyMarkup
            );
        } else {
            json = JsonObject(
                "chat_id", JsonString(userId),
                "message_id", JsonInt(messageId),
                "media", media
            );
        }
        format(endpoint, sizeof(endpoint), "/bot%s/editMessageMedia", BOT_TOKEN);
    }
    else if (strlen(text)) 
    {
        if (strlen(keyboard)) {
            json = JsonObject(
                "chat_id", JsonString(userId),
                "message_id", JsonInt(messageId),
                "text", JsonString(utf8Message),
                "parse_mode", JsonString(parse_mode),
                "reply_markup", replyMarkup
            );
        } else {
            json = JsonObject(
                "chat_id", JsonString(userId),
                "message_id", JsonInt(messageId),
                "text", JsonString(utf8Message),
                "parse_mode", JsonString(parse_mode)
            );
        }
        format(endpoint, sizeof(endpoint), "/bot%s/editMessageText", BOT_TOKEN);
    }
    else 
    {
        json = JsonObject(
            "chat_id", JsonString(userId),
            "message_id", JsonInt(messageId),
            "reply_markup", replyMarkup
        );
        format(endpoint, sizeof(endpoint), "/bot%s/editMessageReplyMarkup", BOT_TOKEN);
    }

    new Request:id = RequestJSON(clientTelegram, endpoint, HTTP_METHOD_POST, "OnPostJson", json, .headers = RequestHeaders());
    return _:id;
}

stock DeleteTelegramMessage(const userId[], messageId[])
{
    if (!botInitialized || !IsValidRequestsClient(clientTelegram)) return 0;

    new endpoint[256], Node:json;
    json = JsonObject(
        "chat_id", JsonString(userId),
        "message_id", JsonString(messageId)
    );

    format(endpoint, sizeof(endpoint), "/bot%s/deleteMessage", BOT_TOKEN);
    new Request:id = RequestJSON(clientTelegram, endpoint, HTTP_METHOD_POST, "OnPostJson", json, .headers = RequestHeaders());
    return _:id;
}

callback OnTelegramResponse(Request:id, E_HTTP_STATUS:status, const data[], dataLen)
{
    new convertData[2096];
    convertData[0] = EOS;
    
    UTF8_To_Win1251(data, convertData, sizeof convertData);

    if (status == HTTP_STATUS_OK)
    {
        new userId[MAX_SIZE_USER_ID];

        ParseJsonInt(convertData, "chat", "id", userId);

        #if DEBUG_MODE_TELEGRAM
            new messageIdStr[32];
            ParseJsonInt(convertData, "result", "message_id", messageIdStr);

            printf("[PawnGram] Message send to user -> %s | bot.message_id -> %s", userId, messageIdStr);
        #endif
    }
    else {
        #if SHOW_ERROR_TELEGRAM_API
        if (status != HTTP_STATUS_OK) {
            new errorOutput[256];
            format(errorOutput, sizeof(errorOutput), "%s", convertData);

            if (!strcmp(SHOW_ERROR_LANGUAGE, "RU", true)) {
                for(new i = 0; i < sizeof TelegramErrorsEN; i++) {
                    if(strfind(convertData, TelegramErrorsEN[i], true) != -1) {
                        format(errorOutput, sizeof(errorOutput), "%s", TelegramErrorsRU[i]);
                        break;
                    }
                }
            }
            else {
                for(new i = 0; i < sizeof TelegramErrorsEN; i++) {
                    if(strfind(convertData, TelegramErrorsEN[i], true) != -1) {
                        format(errorOutput, sizeof(errorOutput), "%s", TelegramErrorsEN[i]);
                        break;
                    }
                }
            }

            printf("[PawnGram -> OnTelegramResponse] ErrorId: %d | Response: %s", _:id, errorOutput);
            return 1;
        }
        #endif
    }

    return 1;
}

stock GetTelegramUserInfo(const userId[])
{   
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;

    new endpoint[256];
    format(endpoint, sizeof(endpoint), "/bot%s/getChat?chat_id=%s", BOT_TOKEN, userId);

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramUserInfo", "");

    if (!IsValidRequest(id))
        return 0;

    #if DEBUG_MODE_TELEGRAM
        printf("[PawnGram] Requesting info for user %s | id -> %d", userId, _:id);
    #endif
    return _:id;
}

callback OnTelegramUserInfo(Request:id, E_HTTP_STATUS:status, const data[], dataLen)
{
    new converteData[2096];
    converteData[0] = EOS;
    
    UTF8_To_Win1251(data, converteData, sizeof converteData);

    if (status == HTTP_STATUS_OK)
    {   
        new firstName[MAX_SIZE_TELEGRAM_NAME], lastName[MAX_SIZE_TELEGRAM_NAME], chatId[64], 
            outputFirstName[MAX_SIZE_TELEGRAM_NAME * 2], outputLastName[MAX_SIZE_TELEGRAM_NAME * 2], 
            userName[MAX_SIZE_USER_NAME];

        ParseJsonString(converteData, "result", "first_name", firstName, sizeof(firstName));
        ParseJsonString(converteData, "result", "last_name", lastName, sizeof(lastName));
        ParseJsonString(converteData, "result", "username", userName, sizeof(userName));
        ParseJsonInt(converteData, "result", "id", chatId);

        DecodeUnicodeEscapes(lastName, outputLastName, sizeof(outputLastName));
        DecodeUnicodeEscapes(firstName, outputFirstName, sizeof(outputFirstName));

        #if DEBUG_MODE_TELEGRAM
            printf("[PawnGram -> OnTelegramUserInfo] Info userId -> %s | first_name -> %s | last_name -> %s | @username -> %s", 
                chatId, outputFirstName, outputLastName, userName);
        #endif
    }
}

callback GetTelegramUpdates()
{
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    new endpoint[256];

    if (lastUpdateId > 0)
        format(endpoint, sizeof(endpoint), "/bot%s/getUpdates?timeout=30&offset=%d", BOT_TOKEN, lastUpdateId + 1);
    else
        format(endpoint, sizeof(endpoint), "/bot%s/getUpdates?timeout=30", BOT_TOKEN);

    if (!IsValidRequestsClient(clientTelegram)) return 0;

    new Request:id = RequestJSON(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramUpdatesJSON");

    if (!IsValidRequest(id))
        return 0;

    return _:id;
}

stock AnswerPreCheckoutQuery(const queryId[], bool:ok, const errorMessage[] = "")
{
    if (!botInitialized || !IsValidRequestsClient(clientTelegram)) return 0;

    new Node:json, endpoint[256];
    if (ok) {
        json = JsonObject("pre_checkout_query_id", JsonString(queryId), "ok", JsonBool(true));
    } else {
        new utf8Error[256];
        ConvertWindows1251ToUTF8(errorMessage, utf8Error, sizeof(utf8Error));
        json = JsonObject("pre_checkout_query_id", JsonString(queryId), "ok", JsonBool(false), "error_message", JsonString(utf8Error));
    }

    format(endpoint, sizeof(endpoint), "/bot%s/answerPreCheckoutQuery", BOT_TOKEN);
    new Request:id = RequestJSON(clientTelegram, endpoint, HTTP_METHOD_POST, "OnPostJson", json, .headers = RequestHeaders());
    return _:id;
}

callback OnTelegramUpdatesJSON(Request:id, E_HTTP_STATUS:status, Node:node) 
{
    if (status != HTTP_STATUS_OK) 
        return 0;

    new jsonInfo[4096], convertData[4096];
    JsonStringify(node, jsonInfo, sizeof jsonInfo);
    UTF8_To_Win1251(jsonInfo, convertData, sizeof convertData);

    new bool:ok, Node:result, len;

    if (JsonGetBool(node, "ok", ok) || !ok)
        return printf("[PawnGram -> OnTelegramUpdatesJSON] Bad JSON response");

    if (JsonGetArray(node, "result", result))
        return printf("[PawnGram -> OnTelegramUpdatesJSON] No result array");

    JsonArrayLength(result, len);

    for (new i = 0; i < len; i++) 
    {        
        new Node:update, updateId;
        JsonArrayObject(result, i, update); 
        JsonGetInt(update, "update_id", updateId); 

        if (updateId <= lastUpdateId)
            continue;

        switch(GetUpdateTypeStr(convertData))
        {
            case GET_UPDATE_MESSAGE: {
                new Node:message, Node:from;
                new text[256], textConverted[512], userId[MAX_SIZE_USER_ID];
                new userName[MAX_SIZE_USER_NAME], firstName[MAX_SIZE_TELEGRAM_NAME], lastName[MAX_SIZE_TELEGRAM_NAME];
                new outputFirstName[MAX_SIZE_TELEGRAM_NAME * 2], outputLastName[MAX_SIZE_TELEGRAM_NAME * 2];

                JsonGetObject(update, "message", message);
                JsonGetString(message, "text", text);
                UTF8_To_Win1251(text, textConverted, sizeof textConverted);

                JsonGetObject(message, "from", from);
                JsonGetString(from, "username", userName);
                JsonGetString(from, "first_name", firstName);
                JsonGetString(from, "last_name", lastName);

                UTF8_To_Win1251(userName, userName, sizeof userName);
                UTF8_To_Win1251(firstName, outputFirstName, sizeof outputFirstName);
                UTF8_To_Win1251(lastName, outputLastName, sizeof outputLastName);

                ParseJsonId(convertData, userId);
                OnTelegramMessage(userId, userName, textConverted, outputFirstName, outputLastName);
            }

            case GET_UPDATE_INLINE_KEYBOARD: {
                new Node:callback, Node:from;
                new callbackData[256], callbackId[64], userId[MAX_SIZE_USER_ID];
                new userName[MAX_SIZE_USER_NAME], firstName[MAX_SIZE_TELEGRAM_NAME], lastName[MAX_SIZE_TELEGRAM_NAME];
                new outputFirstName[MAX_SIZE_TELEGRAM_NAME * 2], outputLastName[MAX_SIZE_TELEGRAM_NAME * 2];

                JsonGetObject(update, "callback_query", callback);
                JsonGetString(callback, "data", callbackData);
                JsonGetString(callback, "id", callbackId);
                UTF8_To_Win1251(callbackData, callbackData, sizeof callbackData);

                JsonGetObject(callback, "from", from);
                JsonGetString(from, "username", userName);
                JsonGetString(from, "first_name", firstName);
                JsonGetString(from, "last_name", lastName);

                UTF8_To_Win1251(userName, userName, sizeof userName);
                UTF8_To_Win1251(firstName, outputFirstName, sizeof outputFirstName);
                UTF8_To_Win1251(lastName, outputLastName, sizeof outputLastName);

                ParseJsonId(convertData, userId);
                OnTelegramInlineKeyBoard(userId, userName, callbackData, outputFirstName, outputLastName, callbackId);
            }

            case GET_UPDATE_SUCCESSFUL_PAYMENTS: {
                new userId[MAX_SIZE_USER_ID], payload[128], currency[10], totalAmount[32], amountStr[32];
                ParseJsonId(convertData, userId);
                ParseJsonString(convertData, "successful_payment", "invoice_payload", payload, sizeof payload);
                ParseJsonString(convertData, "successful_payment", "currency", currency, sizeof currency);
                ParseJsonInt(convertData, "successful_payment", "total_amount", totalAmount);

                format(amountStr, sizeof amountStr, 
                    (strval(totalAmount) * 100) % 100 == 0 ? "%d" : "%d.%02d", strval(totalAmount), (strval(totalAmount) * 100) % 100);

                OnTelegramSuccessfulPayment(userId, payload, currency, amountStr);
            }

            case GET_UPDATE_ANSWER_PAYMENTS: {
                new queryId[64];

                ParseJsonString(convertData, "pre_checkout_query", "id", queryId, sizeof queryId);
                AnswerPreCheckoutQuery(queryId, true);
            }
        }

        lastUpdateId = updateId;
    }

    return 1;
}

stock AnswerCallbackQuery(callbackId[], text[] = "", bool:showAlert = false)
{
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    new Node:json;

    if (strlen(text) > 0)
    {   
        new utf8Message[1024];
        ConvertWindows1251ToUTF8(text, utf8Message, sizeof utf8Message);

        json = JsonObject(
            "callback_query_id", JsonString(callbackId),
            "text", JsonString(utf8Message),
            "show_alert", JsonBool(showAlert)
        );
    }
    else
    {
        json = JsonObject("callback_query_id", JsonString(callbackId));
    }

    new Request:id = RequestJSON(clientTelegram, "/bot"BOT_TOKEN"/answerCallbackQuery", HTTP_METHOD_POST, "OnPostJson", json, .headers = RequestHeaders());

    if (!IsValidRequest(id))
        return 0;

    #if DEBUG_MODE_TELEGRAM
        printf("[PawnGram] AnswerCallbackQuery sent | callback_id -> %s | request_id -> %d", callbackId, _:id);
    #endif

    return _:id;
}

stock GetChatMemberStatus(const chatId[], const userId[])
{   
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;

    new endpoint[256];
    format(endpoint, sizeof endpoint , "/bot%s/getChatMember?chat_id=%s&user_id=%s", BOT_TOKEN, chatId, userId);

    new Request:reqId = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnChatMemberStatus");
    return _:reqId;
}

callback OnChatMemberStatus(Request:reqId, E_HTTP_STATUS:status, const data[], dataLen)
{   
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    new convertData[1024];
    convertData[0] = EOS;

    UTF8_To_Win1251(data, convertData, sizeof convertData);

    if (status == HTTP_STATUS_OK)
    {
        new statusStr[64], userIdOut[64];
        
        ParseJsonString(convertData, "result", "status", statusStr, sizeof statusStr);
        ParseJsonInt(convertData, "user", "id", userIdOut);

        #if DEBUG_MODE_TELEGRAM
            printf("[PawnGram -> OnChatMemberStatus] User %s -> status: %s", userIdOut, statusStr);
        #endif
    }
    else
    {   
        #if SHOW_ERROR_TELEGRAM_API
            printf("[PawnGram -> OnChatMemberStatus] Error %d | id: %d | Response: %s", _:status, _:reqId, convertData);
        #endif
    }

    return 1;
}

stock GetBotInfo()
{
    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;

    new endpoint[256];
    format(endpoint, sizeof endpoint, "/bot%s/getMe", BOT_TOKEN);

    new Request:reqId = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnBotInfo", "");
    return _:reqId;
}

callback OnBotInfo(Request:reqId, E_HTTP_STATUS:status, const data[], dataLen)
{
    new convertData[1024];
    convertData[0] = EOS;
    UTF8_To_Win1251(data, convertData, sizeof(convertData));

    if (status == HTTP_STATUS_OK)
    {
        new botId[64], firstName[MAX_SIZE_TELEGRAM_NAME], userName[MAX_SIZE_TELEGRAM_NAME];
        
        ParseJsonInt(convertData, "result", "id", botId);
        ParseJsonString(convertData, "result", "first_name", firstName, sizeof(firstName));
        ParseJsonString(convertData, "result", "username", userName, sizeof(userName));

        botInitialized = true;

        SetTimer("GetTelegramUpdates", TELEGRAM_TICKRATE_UPDATE, true);

        printf("[PawnGram] Bot successful Initialized! t.me/%s | Name: %s", userName, firstName);
    }   
    else
    {
        printf("[PawnGram -> OnBotInfo] Error %d | id: %d | Response: %s", _:status, _:reqId, convertData);
        botInitialized = false;
    }
}

enum ButtonInfo {
    BtnText[MAX_CALLBACK_SIZE],
    BtnCallback[MAX_CALLBACK_SIZE]
};

stock BuildInlineKeyboard(const buttons[][][MAX_CALLBACK_SIZE], buttonCount, buttonsPerRow, output[], len = sizeof(output))
{
    new Node:rows = JsonArray();
    
    for (new i = 0; i < buttonCount; i += buttonsPerRow)
    {
        new Node:row = JsonArray();
        
        for (new j = 0; j < buttonsPerRow && (i + j) < buttonCount; j++)
        {
            new utf8Text[MAX_CALLBACK_SIZE], utf8Callback[MAX_CALLBACK_SIZE];
            ConvertWindows1251ToUTF8(buttons[i + j][0], utf8Text, sizeof(utf8Text));
            ConvertWindows1251ToUTF8(buttons[i + j][1], utf8Callback, sizeof(utf8Callback));
            
            new Node:button = JsonObject(
                "text", JsonString(utf8Text),
                "callback_data", JsonString(utf8Callback)
            );
            
            new Node:tempRow = JsonArray(button);
            row = JsonAppend(row, tempRow);
        }
        
        new Node:tempRows = JsonArray(row);
        rows = JsonAppend(rows, tempRows);
    }
    
    new Node:result = JsonObject("inline_keyboard", rows);
    JsonStringify(result, output, len);
}


callback OnPostJson(Request:id, E_HTTP_STATUS:status, Node:node) {
    #if SHOW_ERROR_TELEGRAM_API
        if (status != HTTP_STATUS_OK) {
            new description[MAX_CALLBACK_SIZE];
            JsonGetString(node, "description", description);

            new errorOutput[MAX_CALLBACK_SIZE];
            format(errorOutput, sizeof(errorOutput), "%s", description);

            if (!strcmp(SHOW_ERROR_LANGUAGE, "RU", true)) {
                for(new i = 0; i < sizeof TelegramErrorsEN; i++) {
                    if(strfind(description, TelegramErrorsEN[i], true) != -1) {
                        format(errorOutput, sizeof(errorOutput), "%s", TelegramErrorsRU[i]);
                        break;
                    }
                }
            }
            else {
                for(new i = 0; i < sizeof TelegramErrorsEN; i++) {
                    if(strfind(description, TelegramErrorsEN[i], true) != -1) {
                        format(errorOutput, sizeof(errorOutput), "%s", TelegramErrorsEN[i]);
                        break;
                    }
                }
            }


            printf("[PawnGram -> OnPostJson] ErrorId: %d | Response: %s", _:id, errorOutput);
            return 1;
        }
    #endif

    #if DEBUG_MODE_TELEGRAM
        new jsonInfoDebug[2096], convertDataDebug[2096];
        JsonStringify(node, jsonInfoDebug, sizeof jsonInfoDebug);

        UTF8_To_Win1251(jsonInfoDebug, convertDataDebug, sizeof convertDataDebug);
        print(convertDataDebug);
    #endif
    return 1;
}

public OnGameModeInit()
{
    if (!strlen(BOT_TOKEN))
        return print("[PawnGram] BOT_TOKEN is null.");

    clientTelegram = RequestsClient(API_TELEGRAM_URL, RequestHeaders());

    if (!IsValidRequestsClient(clientTelegram)) 
        return print("[PawnGram] Client is invalid.");

    botInitialized = false;

    GetBotInfo();
    #if defined OnGameModeInit_Telegram
        return OnGameModeInit_Telegram();
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit OnGameModeInit_Telegram

#if defined OnGameModeInit_Telegram
    forward OnGameModeInit_Telegram();
#endif