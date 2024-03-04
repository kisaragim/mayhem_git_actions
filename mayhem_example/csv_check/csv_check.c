#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSV_CHECK_ON      1
#define CSV_CHECK_OFF     0

typedef struct {
    int year ;
    int month ;
    int day ;
    int hour ;
    int minute ;
    int sec ;
} _DATE_FORMAT ;

void ReplaceIllegalData(int chk_method,
                        unsigned char *element,
                        int size,
                        int null_chk,
                        int max_byte) ;


void ReplaceIllegalData(int chk_method,
                        unsigned char *element,
                        int size,
                        int null_chk,
                        int max_byte)
{
    int i ;
    int illegal_flg ;        /* 不正データ発見フラグ */
    unsigned char buf[5] ;   /* 日付格納バッファ */
    static int sIllegal_data_cnt ;
    _DATE_FORMAT chk_date ;  /* チェック日付格納構造体 */

    /* フラグ初期化 */
    illegal_flg = CSV_CHECK_OFF ;

    /* チェック内容によらず「データ無」許可で、「データ無」なら要素はそのまま */
    /* 末尾要素は改行を含むため、改行のみでも許可 */
    /* 「データ無」不可でデータがなかった場合、不正データありとする */
    if ( ( size == 0 ) ||
         ( ( size == 1 ) && 
           ( element[0] == (unsigned char)0x0a ) ) )
    {
        if ( null_chk == CSV_CHECK_ON )
        {
            return ;
        }
        else
        {
            illegal_flg = CSV_CHECK_ON ;
        }
    }

    /* チェック内容によらず許容バイト数を超えた場合、不正データ */
    if ( size > max_byte )
    {
        illegal_flg = CSV_CHECK_ON ;
    }

    switch( chk_method )
    {
        case 1:    /* 数値範囲チェック */
            for( i = 0; i < size; i++ )
            {
                /* 半角数字以外を検出(改行は許可) */
                if ( !( ( ( element[i] >= (unsigned char)0x30 ) &&     /* 半角数字 */
                           ( element[i] <= (unsigned char)0x39 ) ) ||  /* 半角数字 */
                        ( element[i] == (unsigned char)0x0a ) ) )      /* 改行 */
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }
            }

            /* 半角数字であった場合、数値範囲チェック */
            if ( i == size )
            {
                if ( ( atoi(element) < 0 ) ||
                     ( atoi(element) > 30000) )
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                }
            }
            break ;

        case 2:    /* 文字列チェック */
            if ( ( strcmp(element, "bug") != 0 ) &&
                 ( strcmp(element, "bua") != 0 ) &&
                 ( strcmp(element, "bub") != 0 ) )
            {
                /* 不正データあり */
                illegal_flg = CSV_CHECK_ON ;
            }
            break ;

        case 3:    /* 数値チェック */
            for( i = 0; i < size; i++ )
            {
                /* 半角数字以外を検出(改行は許可) */
                if ( !( ( ( element[i] >= (unsigned char)0x30 ) &&     /* 半角数字 */
                           ( element[i] <= (unsigned char)0x39 ) ) ||  /* 半角数字 */
                        ( element[i] == (unsigned char)0x0a ) ) )      /* 改行 */
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }
            }
            break ;

        case 4:    /* 半角英数字チェック */
            for( i = 0; i < size; i++ )
            {
                /* 半角英数字以外を検出(改行は許可) */
                if ( !( ( ( element[i] >= (unsigned char)0x30 ) &&     /* 半角数字 */
                           ( element[i] <= (unsigned char)0x39 ) ) ||  /* 半角数字 */
                        ( ( element[i] >= (unsigned char)0x41 ) &&     /* 半角英大文字 */
                           ( element[i] <= (unsigned char)0x5a ) ) ||  /* 半角英大英字 */
                        ( ( element[i] >= (unsigned char)0x61 ) &&     /* 半角英小文字 */
                           ( element[i] <= (unsigned char)0x7a ) ) ||  /* 半角英小文字 */
                        ( element[i] == (unsigned char)0x0a ) ) )      /* 改行 */
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }
            }
            break ;

        case 5:    /* 日付チェック */
            for( i = 0; i < size; i++ )
            {
                /* 半角数字以外を検出(改行は許可) */
                if ( !( ( ( element[i] >= (unsigned char)0x30 ) &&     /* 半角数字 */
                           ( element[i] <= (unsigned char)0x39 ) ) ||  /* 半角数字 */
                        ( element[i] == (unsigned char)0x0a ) ) )      /* 改行 */
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }
            }

            /* 半角数字、かつ、YYYYMMDDhhmmssの桁数であった場合、存在する日付かチェック */
            if ( ( i == size ) &&
                 ( size == 14 ) )
            {
                memset(buf, '\0', sizeof(buf)) ;
                /* YYYYをコピー */
                memmove(buf, element, 4) ;
                chk_date.year = atoi(buf) ;
                memset(buf, '\0', sizeof(buf)) ;
                /* MMをコピー */
                memmove(buf, element+4, 2) ;
                chk_date.month = atoi(buf) ;
                memset(buf, '\0', sizeof(buf)) ;
                /* DDをコピー */
                memmove(buf, element+6, 2) ;
                chk_date.day = atoi(buf) ;
                memset(buf, '\0', sizeof(buf)) ;
                /* hhをコピー */
                memmove(buf, element+8, 2) ;
                chk_date.hour = atoi(buf) ;
                memset(buf, '\0', sizeof(buf)) ;
                /* mmをコピー */
                memmove(buf, element+10, 2) ;
                chk_date.minute = atoi(buf) ;
                memset(buf, '\0', sizeof(buf)) ;
                /* ssをコピー */
                memmove(buf, element+12, 2) ;
                chk_date.sec = atoi(buf) ;

                /* ありえない日付（月によらず）のチェック */
                if ( ( chk_date.month < 1 ) ||
                     ( chk_date.month > 12 ) ||
                     ( chk_date.day < 1 ) ||
                     ( chk_date.day > 31 ) ||
                     ( chk_date.hour > 24 ) ||
                     ( chk_date.minute > 60 ) ||
                     ( chk_date.sec > 60 ) )
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                }
                /* 月によって、ありえない日付のチェック(28～30日の月) */
                else
                {
                    if ( ( ( chk_date.month == 4 ) ||
                           ( chk_date.month == 6 ) ||
                           ( chk_date.month == 9 ) ||
                           ( chk_date.month == 11 ) ) &&
                         ( chk_date.day > 30 ) )
                    {
                        /* 不正データあり */
                        illegal_flg = CSV_CHECK_ON ;
                    }
                    else if ( chk_date.month == 2 )
                    {
                        /* うるう年の場合 */
                        if ( ( chk_date.year % 400 == 0 ) ||
                             ( ( chk_date.year % 4 == 0 ) &&
                               ( chk_date.year % 100 != 0 ) ) )
                        {
                            if ( chk_date.day > 29 )
                            {
                                /* 不正データあり */
                                illegal_flg = CSV_CHECK_ON ;
                            }
                        }
                        else
                        {
                            if ( chk_date.day > 28 )
                            {
                                /* 不正データあり */
                                illegal_flg = CSV_CHECK_ON ;
                            }
                        }
                    }
                }
            }
            break ;

        case 6:    /* 電話番号チェック */
            for( i = 0; i < size; i++ )
            {
                /* 半角英数字、「 # 」「 * 」以外を検出(改行は許可) */
                if ( !( ( ( element[i] >= (unsigned char)0x30 ) &&     /* 半角数字 */
                           ( element[i] <= (unsigned char)0x39 ) ) ||  /* 半角数字 */
                        ( element[i] == (unsigned char)0x23 ) ||       /* 「#」 */
                        ( element[i] == (unsigned char)0x2a ) ||       /* 「*」 */
                        ( element[i] == (unsigned char)0x0a ) ) )      /* 改行 */
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }
            }
            break ;

        case 7:    /* サブスクライバIDチェック */
            for( i = 0; i < size; i++ )
            {
                /* 半角英数字、「 . 」「 _ 」以外を検出(改行は許可) */
                if ( !( ( ( element[i] >= (unsigned char)0x30 ) &&     /* 半角数字 */
                           ( element[i] <= (unsigned char)0x39 ) ) ||  /* 半角数字 */
                        ( ( element[i] >= (unsigned char)0x41 ) &&     /* 半角英大文字 */
                           ( element[i] <= (unsigned char)0x5a ) ) ||  /* 半角英大英字 */
                        ( ( element[i] >= (unsigned char)0x61 ) &&     /* 半角英小文字 */
                           ( element[i] <= (unsigned char)0x7a ) ) ||  /* 半角英小文字 */
                        ( element[i] == (unsigned char)0x2e ) ||       /* 「.」 */
                        ( element[i] == (unsigned char)0x5f ) ||       /* 「_」 */
                        ( element[i] == (unsigned char)0x0a ) ) )      /* 改行 */
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }
            }
            break ;

        case 8:    /* デバイスタイプチェック */
            for( i = 0; i < size; i++ )
            {
                /* 半角英数字、「 - 」以外を検出(改行は許可) */
                if ( !( ( ( element[i] >= (unsigned char)0x30 ) &&     /* 半角数字 */
                          ( element[i] <= (unsigned char)0x39 ) ) ||   /* 半角数字 */
                        ( ( element[i] >= (unsigned char)0x41 ) &&     /* 半角英大文字 */
                          ( element[i] <= (unsigned char)0x5a ) ) ||   /* 半角英大英字 */
                        ( ( element[i] >= (unsigned char)0x61 ) &&     /* 半角英小文字 */
                          ( element[i] <= (unsigned char)0x7a ) ) ||   /* 半角英小文字 */
                        ( element[i] == (unsigned char)0x2d ) ||       /* 「-」 */
                        ( element[i] == (unsigned char)0x0a ) ) )      /* 改行 */
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }
            }
            break ;


        case 9:    /* パスワードチェック */
            for( i = 0; i < size; i++ )
            {
                /* 半角英数字、「 . 」「 - 」「 _ 」「 + 」「 $ 」「 : 」「 % 」「 @ 」「 ! 」「 = 」「 ? 」以外を検出(改行は許可) */
                if ( !( ( ( element[i] >= (unsigned char)0x30 ) &&     /* 半角数字 */
                           ( element[i] <= (unsigned char)0x39 ) ) ||  /* 半角数字 */
                        ( ( element[i] >= (unsigned char)0x40 ) &&     /* 「@」、半角英大文字 */
                           ( element[i] <= (unsigned char)0x5a ) ) ||  /* 「@」、半角英大英字 */
                        ( ( element[i] >= (unsigned char)0x61 ) &&     /* 半角英小文字 */
                           ( element[i] <= (unsigned char)0x7a ) ) ||  /* 半角英小文字 */
                        ( element[i] == (unsigned char)0x2d ) ||       /* 「-」 */
                        ( element[i] == (unsigned char)0x5f ) ||       /* 「_」 */
                        ( element[i] == (unsigned char)0x2b ) ||       /* 「+」 */
                        ( element[i] == (unsigned char)0x24 ) ||       /* 「$」 */
                        ( element[i] == (unsigned char)0x3a ) ||       /* 「:」 */
                        ( element[i] == (unsigned char)0x25 ) ||       /* 「%」 */
                        ( element[i] == (unsigned char)0x21 ) ||       /* 「!」 */
                        ( element[i] == (unsigned char)0x3d ) ||       /* 「=」 */
                        ( element[i] == (unsigned char)0x3f ) ||       /* 「?」 */
                        ( element[i] == (unsigned char)0x0a ) ) )      /* 改行 */
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }
            }
            break ;

        case 10:    /* メールアドレスチェック */
            for( i = 0; i < size; i++ )
            {
                /* 半角英数字、「 - 」「 @ 」「 _ 」「 . 」以外を検出(改行は許可) */
                if ( !( ( ( element[i] >= (unsigned char)0x30 ) &&     /* 半角数字 */
                           ( element[i] <= (unsigned char)0x39 ) ) ||  /* 半角数字 */
                        ( ( element[i] >= (unsigned char)0x40 ) &&     /* 「@」、半角英大文字 */
                           ( element[i] <= (unsigned char)0x5a ) ) ||  /* 「@」、半角英大英字 */
                        ( ( element[i] >= (unsigned char)0x61 ) &&     /* 半角英小文字 */
                           ( element[i] <= (unsigned char)0x7a ) ) ||  /* 半角英小文字 */
                        ( element[i] == (unsigned char)0x2d ) ||       /* 「-」 */
                        ( element[i] == (unsigned char)0x2e ) ||       /* 「.」 */
                        ( element[i] == (unsigned char)0x5f ) ||       /* 「_」 */
                        ( element[i] == (unsigned char)0x0a ) ) )      /* 改行 */
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }
            }
            break ;

        case 11:    /* JPチェック */
            /* 禁則文字、「 # 」「 + 」「 " 」「 \ 」「 < 」「 > 」「 ; 」「 、 」を含まないことをチェック */
            for( i = 0; i < size; i++ )
            {
                /* 不正文字を検出 */
                if ( !( ( element[i] == (unsigned char)0x20 ) ||         /* 半角スペース */
                        ( element[i] == (unsigned char)0x21 ) ||         /* 「!」*/
                        ( element[i] == (unsigned char)0x24 ) ||         /* 「$」*/
                        ( element[i] == (unsigned char)0x25 ) ||         /* 「%」*/
                        ( element[i] == (unsigned char)0x26 ) ||         /* 「&」*/
                        ( element[i] == (unsigned char)0x28 ) ||         /* 「(」*/
                        ( element[i] == (unsigned char)0x29 ) ||         /* 「)」*/
                        ( element[i] == (unsigned char)0x2a ) ||         /* 「*」*/
                        ( ( ( element[i] >= (unsigned char)0x2d ) &&     /* 「<」、「>」、「\」を除いた半角英文字、各種記号 */
                            ( element[i] <= (unsigned char)0x7e ) ) &&
                            ( ( element[i] != (unsigned char)0x3c ) ||
                              ( element[i] != (unsigned char)0x3e ) ||
                              ( element[i] != (unsigned char)0x5c ) ) ) ||
                        ( ( ( element[i] >= (unsigned char)0x81 ) &&     /* 「？」を除いた全角各種記号、英数字、ひらがな */
                            ( element[i] <= (unsigned char)0x82 ) ) &&
                            ( !( ( element[i] == (unsigned char)0x81 ) &&
                                 ( element[i+1] == (unsigned char)0x48 ) ) ) ) ||
                        ( ( element[i] == (unsigned char)0x83 ) &&       /* 全角カタカナ */
                          ( ( element[i+1] >= (unsigned char)0x40 ) &&
                            ( element[i+1] <= (unsigned char)0x9e ) ) ) ||
                        ( ( ( element[i] >= (unsigned char)0x89 ) &&     /* JIS第一水準・第二水準漢字（16区～84区） */
                            ( element[i] <= (unsigned char)0x9f ) ) ||
                          ( ( element[i] >= (unsigned char)0xe0 ) &&
                            ( element[i] <= (unsigned char)0xe9 ) ) ) ||
                        ( ( element[i] == (unsigned char)0x88 ) &&
                          ( ( element[i+1] >= (unsigned char)0x9f ) &&
                            ( element[i+1] <= (unsigned char)0xfc ) ) ) ||
                        ( ( element[i] == (unsigned char)0xea ) &&
                          ( ( element[i+1] >= (unsigned char)0x40 ) &&
                            ( element[i+1] <= (unsigned char)0xa4 ) ) ) ) )
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }

                /* ２バイト文字だった場合、バイトを１つ進める */
                if ( ( ( element[i] >= (unsigned char)0x80 ) &&
                       ( element[i] <= (unsigned char)0x9f ) ) ||
                       ( element[i] >= (unsigned char)0xe0 ) )
                {
                    i++;
                }
            }
            break ;

        case 12:    /* JPEチェック */
            for( i = 0; i < size; i++ )
            {
                /* 制御コード、「'」「,」、半角カナ、「？」、ユーザ外字を検出 */
                if ( ( ( element[i] >= (unsigned char)0x01 ) &&    /* 制御コード */
                       ( element[i] <= (unsigned char)0x1f ) ) ||
                     ( element[i] == (unsigned char)0x27 ) ||      /* 「'」 */
                     ( element[i] == (unsigned char)0x2c ) ||      /* 「,」 */
                     ( element[i] == (unsigned char)0x7f ) ||      /* 制御コード */
                     ( element[i] == (unsigned char)0x00 ) ||      /* 制御コード */
                     ( ( element[i] >= (unsigned char)0xa1 ) &&    /* 半角カナ */
                       ( element[i] <= (unsigned char)0xdf ) ) ||
                     ( ( element[i] == (unsigned char)0x81 ) &&    /* 「？」 */
                       ( element[i+1] == (unsigned char)0x48 ) ) ||
                     ( ( element[i] >= (unsigned char)0xf0 ) &&    /* ユーザ外字 */
                       ( element[i] <= (unsigned char)0xf9 ) ) )
                {
                    /* 不正データあり */
                    illegal_flg = CSV_CHECK_ON ;
                    break ;
                }

                /* ２バイト文字だった場合、バイトを１つ進める */
                if ( ( ( element[i] >= (unsigned char)0x80 ) &&
                       ( element[i] <= (unsigned char)0x9f ) ) ||
                       ( element[i] >= (unsigned char)0xe0 ) )
                {
                    i++;
                }
            }
            break ;

        default :
            break ;
    }

    /* 不正データが存在した場合 */
    if ( illegal_flg == CSV_CHECK_ON )
    {
        /* 不正データを示す文字列に置き換え、不正データ数をインクリメント */
        sIllegal_data_cnt++ ;
    }
}

