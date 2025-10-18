/*
    * github.com/jr1us.com
    * (c) 2025 

    Converts Windows-1251 encoded strings to UTF-8 and vice versa.
    Also includes URL encoding/decoding and JSON parsing functions.

    Example usage:
        - ConvertWindows1251ToUTF8("Your Windows-1251 string", utf8Buffer, sizeof(utf8Buffer));
        - UTF8_To_Win1251("Your UTF-8 string", win1251Buffer, sizeof(win1251Buffer));
        - UrleanCodeExport("String to encode", encodedBuffer, sizeof(encodedBuffer));
        - DecodeUnicodeEscapes("String with \\uXXXX escapes", decodedBuffer, sizeof(decodedBuffer));
        - ParseJson(jsonString, "object", "key", outputBuffer, sizeof(outputBuffer));

    Note: Ensure that the output buffers are sufficiently large to hold the results.

*/

static const table[128] = {
    0x0402, 0x0403, 0x201A, 0x0453, 0x201E, 0x2026, 0x2020, 0x2021,
    0x20AC, 0x2030, 0x0409, 0x2039, 0x040A, 0x040C, 0x040B, 0x040F,
    0x0452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
    0x0000, 0x2122, 0x0459, 0x203A, 0x045A, 0x045C, 0x045B, 0x045F,
    0x00A0, 0x040E, 0x045E, 0x0408, 0x00A4, 0x0490, 0x00A6, 0x00A7,
    0x0401, 0x00A9, 0x0404, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x0407,
    0x00B0, 0x00B1, 0x0406, 0x0456, 0x0491, 0x00B5, 0x00B6, 0x00B7,
    0x0451, 0x2116, 0x0454, 0x00BB, 0x0458, 0x0405, 0x0455, 0x0457,
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
    0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
    0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
    0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
    0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F
};


stock UTF8_To_Win1251(const input[], output[], size = sizeof output)
{
    new i = 0, j = 0;
    while (input[i] && j < size - 1) {
        if (input[i] >= 0xD0 && input[i] <= 0xD1) {
            if (input[i] == 0xD0 && input[i+1] >= 0x90 && input[i+1] <= 0xBF) {
                output[j++] = input[i+1] - 0x90 + 0xC0;
                i += 2;
                continue;
            }
            if (input[i] == 0xD0 && input[i+1] >= 0xB0 && input[i+1] <= 0xBF) {
                output[j++] = input[i+1] - 0xB0 + 0xE0;
                i += 2;
                continue;
            }
            if (input[i] == 0xD1 && input[i+1] >= 0x80 && input[i+1] <= 0x8F) {
                output[j++] = input[i+1] - 0x80 + 0xF0;
                i += 2;
                continue;
            }
        }
        if (input[i] < 0x80) {
            output[j++] = input[i++];
            continue;
        }
        i++;
    }
    output[j] = '\0';
    return j;
}

stock UrleanCodeExport(const input[], output[], size = sizeof(output))
{
    new j = 0, hi, lo;

    for (new i = 0; j < size; i++)
    {
        new c = input[i] & 0xFF;

        if (c == 0) 
            break;

        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~')
            output[j++] = c;
        else if (c == ' ')
            output[j++] = '+';
        else
        {
            if (j + 3 >= size)
                break;

            output[j++] = '%';
            hi = c / 16;
            lo = c % 16;
            output[j++] = (hi < 10) ? ('0' + hi) : ('A' + hi - 10);
            output[j++] = (lo < 10) ? ('0' + lo) : ('A' + lo - 10);
        }
    }

    output[j] = EOS;

    return j;
}

stock ConvertCharToUTF8(c, utf8[])
{
    new codepoint;

    if (c < 0x80)
        codepoint = c;
    else
        codepoint = table[c - 0x80];

    if (codepoint < 0x80) {
        utf8[0] = codepoint;
        return 1;
    } else if (codepoint < 0x800) {
        utf8[0] = 0xC0 | (codepoint >> 6);
        utf8[1] = 0x80 | (codepoint & 0x3F);
        return 2;
    } else {
        utf8[0] = 0xE0 | (codepoint >> 12);
        utf8[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8[2] = 0x80 | (codepoint & 0x3F);
        return 3;
    }
}

stock ConvertWindows1251ToUTF8(const input[], output[], maxlen)
{
    new len = strlen(input), outpos = 0;
    new buf[3], utf_len;

    for (new i = 0; i < len; i++)
    {
        utf_len = ConvertCharToUTF8(input[i], buf);

        if (outpos + utf_len >= maxlen)
            break;

        for (new j = 0; j < utf_len; j++)
            output[outpos++] = buf[j];
    }

    return outpos;
}

stock ishex(c)
{
    return (c >= '0' && c <= '9') || 
           (c >= 'A' && c <= 'F') || 
           (c >= 'a' && c <= 'f');
}

stock DecodeUnicodeEscapes(const input[], output[], size = sizeof output)
{
    new i = 0, j = 0;
    while (input[i] && j < size - 1) {
        if (input[i] == '\\' && input[i+1] == 'u' &&
            ishex(input[i+2]) && ishex(input[i+3]) &&
            ishex(input[i+4]) && ishex(input[i+5])) {

            new hexValue = 0;
            for (new k = 0; k < 4; k++) {
                hexValue *= 16;
                if (input[i+2+k] >= '0' && input[i+2+k] <= '9')
                    hexValue += input[i+2+k] - '0';
                else if (input[i+2+k] >= 'A' && input[i+2+k] <= 'F')
                    hexValue += input[i+2+k] - 'A' + 10;
                else if (input[i+2+k] >= 'a' && input[i+2+k] <= 'f')
                    hexValue += input[i+2+k] - 'a' + 10;
            }

            switch(hexValue) {
                case 0x0410: output[j++] = 0xC0; // À
                case 0x0411: output[j++] = 0xC1; // Á
                case 0x0412: output[j++] = 0xC2; // Â
                case 0x0413: output[j++] = 0xC3; // Ã
                case 0x0414: output[j++] = 0xC4; // Ä
                case 0x0415: output[j++] = 0xC5; // Å
                case 0x0401: output[j++] = 0xA8; // ¨
                case 0x0416: output[j++] = 0xC6; // Æ
                case 0x0417: output[j++] = 0xC7; // Ç
                case 0x0418: output[j++] = 0xC8; // È
                case 0x0419: output[j++] = 0xC9; // É
                case 0x041A: output[j++] = 0xCA; // Ê
                case 0x041B: output[j++] = 0xCB; // Ë
                case 0x041C: output[j++] = 0xCC; // Ì
                case 0x041D: output[j++] = 0xCD; // Í
                case 0x041E: output[j++] = 0xCE; // Î
                case 0x041F: output[j++] = 0xCF; // Ï
                case 0x0420: output[j++] = 0xD0; // Ð
                case 0x0421: output[j++] = 0xD1; // Ñ
                case 0x0422: output[j++] = 0xD2; // Ò
                case 0x0423: output[j++] = 0xD3; // Ó
                case 0x0424: output[j++] = 0xD4; // Ô
                case 0x0425: output[j++] = 0xD5; // Õ
                case 0x0426: output[j++] = 0xD6; // Ö
                case 0x0427: output[j++] = 0xD7; // ×
                case 0x0428: output[j++] = 0xD8; // Ø
                case 0x0429: output[j++] = 0xD9; // Ù
                case 0x042A: output[j++] = 0xDA; // Ú
                case 0x042B: output[j++] = 0xDB; // Û
                case 0x042C: output[j++] = 0xDC; // Ü
                case 0x042D: output[j++] = 0xDD; // Ý
                case 0x042E: output[j++] = 0xDE; // Þ
                case 0x042F: output[j++] = 0xDF; // ß
                case 0x0430: output[j++] = 0xE0; // à
                case 0x0431: output[j++] = 0xE1; // á
                case 0x0432: output[j++] = 0xE2; // â
                case 0x0433: output[j++] = 0xE3; // ã
                case 0x0434: output[j++] = 0xE4; // ä
                case 0x0435: output[j++] = 0xE5; // å
                case 0x0451: output[j++] = 0xB8; // ¸
                case 0x0436: output[j++] = 0xE6; // æ
                case 0x0437: output[j++] = 0xE7; // ç
                case 0x0438: output[j++] = 0xE8; // è
                case 0x0439: output[j++] = 0xE9; // é
                case 0x043A: output[j++] = 0xEA; // ê
                case 0x043B: output[j++] = 0xEB; // ë
                case 0x043C: output[j++] = 0xEC; // ì
                case 0x043D: output[j++] = 0xED; // í
                case 0x043E: output[j++] = 0xEE; // î
                case 0x043F: output[j++] = 0xEF; // ï
                case 0x0440: output[j++] = 0xF0; // ð
                case 0x0441: output[j++] = 0xF1; // ñ
                case 0x0442: output[j++] = 0xF2; // ò
                case 0x0443: output[j++] = 0xF3; // ó
                case 0x0444: output[j++] = 0xF4; // ô
                case 0x0445: output[j++] = 0xF5; // õ
                case 0x0446: output[j++] = 0xF6; // ö
                case 0x0447: output[j++] = 0xF7; // ÷
                case 0x0448: output[j++] = 0xF8; // ø
                case 0x0449: output[j++] = 0xF9; // ù
                case 0x044A: output[j++] = 0xFA; // ú
                case 0x044B: output[j++] = 0xFB; // û
                case 0x044C: output[j++] = 0xFC; // ü
                case 0x044D: output[j++] = 0xFD; // ý
                case 0x044E: output[j++] = 0xFE; // þ
                case 0x044F: output[j++] = 0xFF; // ÿ
                default: output[j++] = input[i]; 
            }
            i += 6; 
        } else {
            output[j++] = input[i++];
        }
    }
    output[j] = '\0';
    return j;
}

stock ParseJsonString(const input[], const object[], const key[], output[], maxlen)
{
    new startPos = 0, endPos = 0;

    if (object[0] != '\0') {
        new objPattern[128];
        format(objPattern, sizeof(objPattern), "\"%s\":{", object);
        startPos = strfind(input, objPattern, false);
        if (startPos == -1) return 0;
        startPos += strlen(objPattern);
    }

    if (key[0] != '\0') {
        new keyPattern[128];
        format(keyPattern, sizeof(keyPattern), "\"%s\":\"", key);
        new pos = strfind(input[startPos], keyPattern, false);
        if (pos == -1) return 0;
        startPos += pos + strlen(keyPattern);
    }

    for (new i = startPos; input[i] != '\0'; i++) {
        if (input[i] == '"') {
            endPos = i;
            break;
        }
    }

    if (endPos == 0) return 0;

    new len = endPos - startPos;
    if (len >= maxlen) len = maxlen - 1;

    for (new i = 0; i < len; i++)
        output[i] = input[startPos + i];

    return len;
}

stock ParseJsonInt(const input[], const object[], const key[], output[] = "") // back to STRING
{
    new startPos = 0, endPos = 0;
    new len = 0, pos = 0;
    new objects[128], keyPattern[128];

    format(objects, sizeof(objects), "\"%s\":{", object);
    startPos = strfind(input, objects, false);

    if (startPos == -1) return 0;
    startPos += strlen(objects);

    if (key[0] != '\0') {        
        format(keyPattern, sizeof(keyPattern), "\"%s\":", key);
        pos = strfind(input[startPos], keyPattern, false);

        if (pos == -1) return 0;
        startPos += pos + strlen(keyPattern);
    }

    for (new i = startPos; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9') {
            endPos = i;
            break;
        }
    }

    len = endPos - startPos;

    for (new i = 0; i < len; i++)
        output[i] = input[startPos + i];

    return 1;
}

stock ParseJsonId(const input[], output[], maxlen = sizeof(output)) // back to string!
{
    new fromPos = strfind(input, "\"from\":", false);

    if (fromPos == -1) 
        return 0;

    new idPos = strfind(input, "\"id\":", false, fromPos);
    
    if (idPos == -1) 
        return 0;

    idPos += 5; 

    while (input[idPos] == ' ' || input[idPos] == '\t' || input[idPos] == '\r' || input[idPos] == '\n')
        idPos++;
    
    new start = idPos;
    new end = start;

    while (input[end] >= '0' && input[end] <= '9')
        end++;
    
    if (end == start) return 0;

    if (end - start >= maxlen)
        end = start + maxlen - 1;
    

    strmid(output, input, start, end, maxlen);
    return 1;
}

stock EscapeMarkdown(const input[], output[], maxlen = sizeof(output))
{
    new i, j;
    for (i = 0, j = 0; input[i] != EOS && j < maxlen - 2; i++)
    {
        switch (input[i])
        {
            case '_', '*', '[', ']', '(', ')', '~', '`', '>', '#', '+', '-', '=', '|', '{', '}', '.', '!':
            {
                output[j++] = '\\';
                output[j++] = input[i];
            }
            default:
                output[j++] = input[i];
        }
    }
    output[j] = EOS;
}

stock EncodePhotoURL(const url[], encoded[], size = sizeof(encoded))
{
    new len = strlen(url), pos = 0;
    for (new i = 0; i < len && pos < size-1; i++)
    {
        new c = url[i];
        if (c == ' ') { encoded[pos++] = '%'; encoded[pos++] = '2'; encoded[pos++] = '0'; }
        else if (c == '(') { encoded[pos++] = '%'; encoded[pos++] = '2'; encoded[pos++] = '8'; }
        else if (c == ')') { encoded[pos++] = '%'; encoded[pos++] = '2'; encoded[pos++] = '9'; }
        else encoded[pos++] = c;
    }
    encoded[pos] = '\0';
}