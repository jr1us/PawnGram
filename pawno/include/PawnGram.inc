/*
    * github.com/jr1us.com
    * (c) 2025 
    
    * Telegram bots in Pawn.
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

#if !defined DEBUG_MODE_TELEGRAM
    #define DEBUG_MODE_TELEGRAM 0 
#endif

#if !defined SHOW_ERROR_TELEGRAM_API
    #define SHOW_ERROR_TELEGRAM_API 1 
#endif

#define SHOW_ERROR_LANGUAGE "EN" // RU | EN

const MAX_CALLBACK_SIZE = 128;

static bool:botInitialized = false;
static RequestsClient:clientTelegram;
static lastUpdateId = 0; 

static const TelegramErrorsEN[][] = {
    "Bad Request: chat not found",
    "Bad Request: message is not modified",
    "Bad Request: message text is empty",
    "Bad Request: message to delete not found",
    "Bad Request: message can't be deleted",
    "Bad Request: message to edit not found",
    "Bad Request: query is too old",
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
    "Bad Gateway: wrong response from the Telegram API"
};

static const TelegramErrorsRU[][] = {
    "Чат c пользователем не найден",
    "Сообщение не изменено",
    "Текст сообщения пустой",
    "Сообщение для удаления не найдено",
    "Сообщение не может быть удалено",
    "Сообщение для редактирования не найдено",
    "Запрос слишком старый",
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
    "Ошибка шлюза: неверный ответ от API Telegram"
};

stock SendTelegramMessage(const userId[], const text[], const parse_mode[] = "", const photoUrl[] = "", const videoUrl[] = "", const stickerFileId[] = "", const voiceUrl[] = "", const videoNoteUrl[] = "", const keyboard[] = "")
{   
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;

    if (strlen(keyboard)) {
        new utf8Message[4096], fullJson[8192], Node:json;
        new method[64];

        if (strlen(photoUrl)) {
            method = "/sendPhoto";
            ConvertWindows1251ToUTF8(text, utf8Message, sizeof(utf8Message));
            
            if (strlen(parse_mode))
                format(fullJson, sizeof(fullJson), "{\"chat_id\":\"%s\",\"photo\":\"%s\",\"caption\":\"%s\",\"parse_mode\":\"%s\",\"reply_markup\":{\"inline_keyboard\":%s}}", userId, photoUrl, utf8Message, parse_mode, keyboard);
            else
                format(fullJson, sizeof(fullJson), "{\"chat_id\":\"%s\",\"photo\":\"%s\",\"caption\":\"%s\",\"reply_markup\":{\"inline_keyboard\":%s}}", userId, photoUrl, utf8Message, keyboard);
        }
        else if (strlen(videoUrl)) {
            method = "/sendVideo";
            ConvertWindows1251ToUTF8(text, utf8Message, sizeof(utf8Message));
            
            if (strlen(parse_mode))
                format(fullJson, sizeof(fullJson), "{\"chat_id\":\"%s\",\"video\":\"%s\",\"caption\":\"%s\",\"parse_mode\":\"%s\",\"reply_markup\":{\"inline_keyboard\":%s}}", userId, videoUrl, utf8Message, parse_mode, keyboard);
            else
                format(fullJson, sizeof(fullJson), "{\"chat_id\":\"%s\",\"video\":\"%s\",\"caption\":\"%s\",\"reply_markup\":{\"inline_keyboard\":%s}}", userId, videoUrl, utf8Message, keyboard);
        }
        else {
            method = "/sendMessage";
            ConvertWindows1251ToUTF8(text, utf8Message, sizeof(utf8Message));

            if (strlen(parse_mode))
                format(fullJson, sizeof(fullJson), "{\"chat_id\":\"%s\",\"text\":\"%s\",\"parse_mode\":\"%s\",\"reply_markup\":{\"inline_keyboard\":%s}}", userId, utf8Message, parse_mode, keyboard);
            else
                format(fullJson, sizeof(fullJson), "{\"chat_id\":\"%s\",\"text\":\"%s\",\"reply_markup\":{\"inline_keyboard\":%s}}", userId, utf8Message, keyboard);
        }

        JsonParse(fullJson, json);

        new endpoint[128];
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

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramResponse");

    if (!IsValidRequest(id))
        return 0;

    #if DEBUG_MODE_TELEGRAM
        printf("[PawnGram] Request sent to user %s | id -> %d", userId, _:id);
    #endif

    return _:id;
}

callback OnTelegramResponse(Request:id, E_HTTP_STATUS:status, const data[], dataLen)
{
    new convertData[2096];
    convertData[0] = EOS;
    
    UTF8_To_Win1251(data, convertData, sizeof convertData);

    if (status == HTTP_STATUS_OK)
    {
        new userId[32];
        ParseJsonInt(convertData, "chat", "id", userId);

        #if DEBUG_MODE_TELEGRAM
            printf("[PawnGram] Message send to user -> %s", userId);
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
        new firstName[128], lastName[128], chatId[64], outputFirstName[128], outputLastName[512], userName[256];

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

callback OnTelegramUpdatesJSON(Request:id, E_HTTP_STATUS:status, Node:node) 
{
    if (status != HTTP_STATUS_OK) 
        return 0;

    new jsonInfo[2096], convertData[2096];
    new text[256], userId[32], textConverted[512];

    JsonStringify(node, jsonInfo, sizeof jsonInfo);
    UTF8_To_Win1251(jsonInfo, convertData, sizeof convertData);

    new bool:ok;
    new Node:result, Node:update, Node:message, Node:from;
    new len, updateId;
    new firstName[256], lastName[256], outputFirstName[256], outputLastName[256], userName[32];

    if (JsonGetBool(node, "ok", ok) || !ok)
        return printf("[PawnGram -> OnTelegramUpdatesJSON] Bad JSON response");

    if (JsonGetArray(node, "result", result))
        return printf("[PawnGram -> OnTelegramUpdatesJSON] No result array");

    JsonArrayLength(result, len);

    for (new i = 0; i < len; i++) 
    {        
        if (GetUpdateTypeStr(convertData) == 2) {
            JsonArrayObject(result, i, update); 
            JsonGetInt(update, "update_id", updateId); 

            if (updateId <= lastUpdateId)
                continue;

            new Node:callback;
            JsonGetObject(update, "callback_query", callback);

            new callbackData[256];
            new callbackId[64];

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

        if (GetUpdateTypeStr(convertData) == 1) {
            JsonArrayObject(result, i, update); 
            JsonGetInt(update, "update_id", updateId); 

            if (updateId <= lastUpdateId)
                continue;

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
        ConvertWindows1251ToUTF8(text, utf8Message, sizeof(utf8Message));

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
    format(endpoint, sizeof(endpoint), "/bot%s/getChatMember?chat_id=%s&user_id=%s", BOT_TOKEN, chatId, userId);

    new Request:reqId = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnChatMemberStatus");
    return _:reqId;
}

callback OnChatMemberStatus(Request:reqId, E_HTTP_STATUS:status, const data[], dataLen)
{   
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    new convertData[1024];
    convertData[0] = EOS;

    UTF8_To_Win1251(data, convertData, sizeof(convertData));

    if (status == HTTP_STATUS_OK)
    {
        new statusStr[64], userIdOut[64];
        
        ParseJsonString(convertData, "result", "status", statusStr, sizeof(statusStr));
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
    format(endpoint, sizeof(endpoint), "/bot%s/getMe", BOT_TOKEN);

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
        new botId[64], firstName[128], userName[128];
        
        ParseJsonInt(convertData, "result", "id", botId);
        ParseJsonString(convertData, "result", "first_name", firstName, sizeof(firstName));
        ParseJsonString(convertData, "result", "username", userName, sizeof(userName));

        botInitialized = true;

        SetTimer("GetTelegramUpdates", 500, true);

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

stock BuildInlineKeyboard(const buttons[][][MAX_CALLBACK_SIZE], buttonCount, buttonsPerRow, output[], len = sizeof(output)) {
    static result[4096], tmp[MAX_CALLBACK_SIZE * 2], utf8Text[1024];

    result[0] = tmp[0] = utf8Text[0] = EOS;

    strcat(result, "[");

    for (new i = 0; i < buttonCount; i += buttonsPerRow) {
        if (i > 0)
            strcat(result, ",");

        strcat(result, "[");

        for (new j = 0; j < buttonsPerRow && (i + j) < buttonCount; j++) {
            ConvertWindows1251ToUTF8(buttons[i + j][0], utf8Text, sizeof(utf8Text));

            if (j > 0) 
                strcat(result, ",");

            format(tmp, sizeof(tmp), "{\"text\":\"%s\",\"callback_data\":\"%s\"}", utf8Text, buttons[i + j][1]);
            strcat(result, tmp);
        }

        strcat(result, "]");
    }

    strcat(result, "]");

    format(output, len, "%s", result);
}

callback OnPostJson(Request:id, E_HTTP_STATUS:status, Node:node) {
    #if SHOW_ERROR_TELEGRAM_API
        if (status != HTTP_STATUS_OK) {
            new description[MAX_CALLBACK_SIZE * 2];
            JsonGetString(node, "description", description);

            new errorOutput[MAX_CALLBACK_SIZE * 2];
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