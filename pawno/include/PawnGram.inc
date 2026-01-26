/*

    * github.com/jr1us.com
    * (c) 2025 
    
    * Telegram bots in Pawn.
*/
#if !defined callback
    #define callback%0(%1) forward%0(%1);public%0(%1)
#endif

#define BOT_TOKEN "7627136521:AAELZXv6eKzsxRpyAjAW8rCo3TenIyJl2lY"

#if !defined BOT_TOKEN
    #error "[PawnGram] Please create #define BOT_TOKEN "TokenBotFather" "
    #define BOT_TOKEN "" // Token t.me/BotFather
#endif

#define API_TELEGRAM_URL "https://api.telegram.org"

static bool:botInitialized = false;
static RequestsClient:clientTelegram;

static lastUpdateId = 0; 

stock SendTelegramMessage(const userId[], const message[], const parse_mode[] = "")
{   
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;

    new utf8Message[2096], encoded[2096];    
    new endpoint[4096];
    new parseStr[32];

    ConvertWindows1251ToUTF8(message, utf8Message, sizeof(utf8Message));
    UrleanCodeExport(utf8Message, encoded, sizeof(encoded));

    if (strlen(parse_mode))
        format(parseStr, sizeof(parseStr), "&parse_mode=%s", parse_mode);
    else
        parseStr = "";

    format(endpoint, sizeof(endpoint),
        "/bot%s/sendMessage?chat_id=%s&text=%s%s",
        BOT_TOKEN,
        userId,
        encoded,
        parseStr
    );

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramResponse");

    if (!IsValidRequest(id))
        return 0;

    #if defined DEBUG_MODE_TELEGRAM
        printf("[PawnGram] Request sent to user %s | id -> %d", userId, _:id);
    #endif
    return _:id;
}

stock SendTelegramSticker(const userId[], const stickerFileId[])
{
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;

    new endpoint[4096];
    format(endpoint, sizeof(endpoint),
        "/bot%s/sendSticker?chat_id=%s&sticker=%s",
        BOT_TOKEN,
        userId,
        stickerFileId
    );

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramResponse");

    if (!IsValidRequest(id))
        return 0;

    #if defined DEBUG_MODE_TELEGRAM
        printf("[PawnGram] Sticker sent to user %s | id -> %d", userId, _:id);
    #endif

    return _:id;
}

stock SendTelegramPhoto(const userId[], const photoURL[], const caption[] = "", const parse_mode[] = "")
{
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram))
        return 0;

    new utf8URL[2096], encodedURL[2096];
    new utf8Caption[1024], encodedCaption[1024];
    new endpoint[4096];
    new captionStr[2048];

    ConvertWindows1251ToUTF8(photoURL, utf8URL, sizeof(utf8URL));
    UrleanCodeExport(utf8URL, encodedURL, sizeof(encodedURL));

    if (strlen(caption))
    {
        ConvertWindows1251ToUTF8(caption, utf8Caption, sizeof(utf8Caption));
        UrleanCodeExport(utf8Caption, encodedCaption, sizeof(encodedCaption));

        if (strlen(parse_mode))
            format(captionStr, sizeof(captionStr), "&caption=%s&parse_mode=%s", encodedCaption, parse_mode);
        else
            format(captionStr, sizeof(captionStr), "&caption=%s", encodedCaption);
    }
    
    format(endpoint, sizeof(endpoint),
        "/bot%s/sendPhoto?chat_id=%s&photo=%s%s",
        BOT_TOKEN,
        userId,
        encodedURL,
        captionStr
    );

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramResponse");
    if (!IsValidRequest(id))
        return 0;

    #if defined DEBUG_MODE_TELEGRAM
        printf("[PawnGram] Photo sent to chat %s | id -> %d", userId, _:id);
    #endif
    return _:id;
}


stock SendTelegramVoice(const chatId[], const voiceURL[])
{
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;

    new encodedURL[1024];
    EncodePhotoURL(voiceURL, encodedURL, sizeof(encodedURL));

    new endpoint[1024];
    format(endpoint, sizeof(endpoint),
        "/bot%s/sendVoice?chat_id=%s&voice=%s",
        BOT_TOKEN,
        chatId,
        encodedURL
    );

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramResponse");
    if (!IsValidRequest(id))
        return 0;

    return _:id;
}

stock SendTelegramVideo(const userId[], const videoURL[], const caption[] = "", const parse_mode[] = "")
{
    if (!botInitialized) 
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;

    new utf8URL[2096], encodedURL[2096];
    new utf8Caption[1024], encodedCaption[1024];
    new endpoint[4096], captionStr[2048];

    ConvertWindows1251ToUTF8(videoURL, utf8URL, sizeof(utf8URL));
    UrleanCodeExport(utf8URL, encodedURL, sizeof(encodedURL));

    if (strlen(caption))
    {
        ConvertWindows1251ToUTF8(caption, utf8Caption, sizeof(utf8Caption));
        UrleanCodeExport(utf8Caption, encodedCaption, sizeof(encodedCaption));
        if (strlen(parse_mode))
            format(captionStr, sizeof(captionStr), "&caption=%s&parse_mode=%s", encodedCaption, parse_mode);
        else
            format(captionStr, sizeof(captionStr), "&caption=%s", encodedCaption);
    }

    format(endpoint, sizeof(endpoint),
        "/bot%s/sendVideo?chat_id=%s&video=%s%s",
        BOT_TOKEN,
        userId,
        encodedURL,
        captionStr
    );

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramResponse");
    if (!IsValidRequest(id)) return 0;

    #if defined DEBUG_MODE_TELEGRAM
        printf("[PawnGram] Video sent to chat %s | id -> %d", userId, _:id);
    #endif
    return _:id;
}

stock SendTelegramVideoNote(const chatId[], const videoNoteURL[])
{
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram)) 
        return 0;

    new encodedURL[1024];
    EncodePhotoURL(videoNoteURL, encodedURL, sizeof(encodedURL));

    new endpoint[1024];
    format(endpoint, sizeof(endpoint),
        "/bot%s/sendVideoNote?chat_id=%s&video_note=%s",
        BOT_TOKEN,
        chatId,
        encodedURL
    );

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramResponse");
    if (!IsValidRequest(id))
        return 0;

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

        #if defined DEBUG_MODE_TELEGRAM
            printf("[PawnGram] Message send to user -> %s", userId);
        #endif
    }
    else {
        #if defined DEBUG_MODE_TELEGRAM
            printf("[PawnGram -> OnTelegramResponse] Error %d | id: %d | Response: %s", _:status, _:id, convertData);
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
    format(endpoint, sizeof(endpoint),
        "/bot%s/getChat?chat_id=%s",
        BOT_TOKEN,
        userId
    );

    new Request:id = Request(clientTelegram, endpoint, HTTP_METHOD_GET, "OnTelegramUserInfo", "");

    if (!IsValidRequest(id))
        return 0;

    #if defined DEBUG_MODE_TELEGRAM
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
        new firstName[128], lastName[128], chatId[64],
            outputFirstName[128], outputLastName[512],
            userName[256];

        ParseJsonString(converteData, "result", "first_name", firstName, sizeof(firstName));
        ParseJsonString(converteData, "result", "last_name", lastName, sizeof(lastName));

        ParseJsonString(converteData, "result", "username", userName, sizeof(userName));

        ParseJsonInt(converteData, "result", "id", chatId);

        DecodeUnicodeEscapes(lastName, outputLastName, sizeof(outputLastName));
        DecodeUnicodeEscapes(firstName, outputFirstName, sizeof(outputFirstName));

        #if defined DEBUG_MODE_TELEGRAM
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
        format(endpoint, sizeof(endpoint),
            "/bot%s/getUpdates?timeout=30&offset=%d",
            BOT_TOKEN, lastUpdateId + 1
        );
    else
        format(endpoint, sizeof(endpoint),
            "/bot%s/getUpdates?timeout=30",
            BOT_TOKEN
        );

    if (!IsValidRequestsClient(clientTelegram)) return 0;

    new Request:id = RequestJSON(
        clientTelegram,
        endpoint,
        HTTP_METHOD_GET,
        "OnTelegramUpdatesJSON"
    );

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
        json = JsonObject(
            "callback_query_id", JsonString(callbackId)
        );
    }

    new Request:id = RequestJSON(
        clientTelegram,
        "/bot"BOT_TOKEN"/answerCallbackQuery",
        HTTP_METHOD_POST,
        "OnPostJson",
        json,
        .headers = RequestHeaders()
    );

    if (!IsValidRequest(id))
        return 0;

    #if defined DEBUG_MODE_TELEGRAM
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
    
    format(endpoint, sizeof(endpoint),
        "/bot%s/getChatMember?chat_id=%s&user_id=%s",
        BOT_TOKEN, chatId, userId
    );

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

        #if defined DEBUG_MODE_TELEGRAM
            printf("[PawnGram -> OnChatMemberStatus] User %s -> status: %s", userIdOut, statusStr);
        #endif
    }
    else
    {   
        #if defined DEBUG_MODE_TELEGRAM
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

        SetTimer("GetTelegramUpdates", 1000, true);

        printf("[PawnGram] Bot successful Initialized! t.me/%s | Name: %s", userName, firstName);
    }   
    else
    {
        printf("[PawnGram -> OnBotInfo] Error %d | id: %d | Response: %s", _:status, _:reqId, convertData);
        botInitialized = false;
    }
}

stock SendTelegramMessageWithButton(Node:json)
{
    if (!botInitialized)
        return print("[PawnGram] Bot not initialized yet!");

    if (!IsValidRequestsClient(clientTelegram))
        return 0;

    new Request:id = RequestJSON(
        clientTelegram,
        "/bot"BOT_TOKEN"/sendMessage",
        HTTP_METHOD_POST,
        "OnPostJson",
        json,
        .headers = RequestHeaders()
    );

    if (!IsValidRequest(id))
        return 0;

    #if defined DEBUG_MODE_TELEGRAM
        printf("[PawnGram] Request sent id -> %d (OnPostJson)", _:id);
    #endif

    return _:id;
}



callback OnPostJson(Request:id, E_HTTP_STATUS:status, Node:node) {
    #if defined DEBUG_MODE_TELEGRAM
    new jsonInfo[2096], convertData[2096];

    JsonStringify(node, jsonInfo, sizeof jsonInfo);
    UTF8_To_Win1251(jsonInfo, convertData, sizeof convertData);

    print(convertData);
    #endif
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