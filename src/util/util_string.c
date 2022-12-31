//
// Created by dolph on 2022/12/31.
//

#include "util_string.h"

char *replace_char(char* str, const char before, const char after) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '\0') {
            break;
        }
        if (str[i] == before) {
            str[i] = after;
        }
    }
    return str;
}

/*
    文字列を置換する

    引数
        s: 置換対象の文字列。NULL不可
        before: 置き換える文字列。NULL不可。"" の場合は何もしない
        after: 置き換え後の文字列。NULL不可

    戻り値
        s を返す
*/
char* replace(char* s, const char* before, const char* after)
{
    assert(s != NULL);
    assert(before != NULL);
    assert(after != NULL);

    const size_t before_len = strlen(before);
    if (before_len == 0) {
        return s;
    }

    const size_t after_len = strlen(after);
    char* p = s;

    for (;;) {

        // 置換する文字列を探す
        p = strstr(p, before);
        if (p == NULL) {
            // 見つからなければ、これ以上置換するものはないので終了する
            break;
        }

        // 置換対象にならない位置を計算
        const char* p2 = p + before_len;

        // 置換対象にならない位置(p2) 以降の文字列を、
        // 置換の影響を受けない位置に移動
        memmove(p + after_len, p2, strlen(p2) + 1);

        // 置換する
        memcpy(p, after, after_len);

        // 探索開始位置をずらす
        p += after_len;
    }

    return s;
}

void strchg(char *buf, const char *str1, const char *str2)
{
    char tmp[1024 + 1];
    char *p;

    while ((p = strstr(buf, str1)) != NULL) {
        /* 見つからなくなるまで繰り返す
              pは旧文字列の先頭を指している */
        *p = '\0'; /* 元の文字列を旧文字列の直前で区切って */
        p += strlen(str1);  /* ポインタを旧文字列の次の文字へ */
        strcpy(tmp, p);             /* 旧文字列から後を保存 */
        strcat(buf, str2);  /* 新文字列をその後につなぎ */
        strcat(buf, tmp);   /* さらに残りをつなぐ */
    }
}