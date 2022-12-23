//
// Created by dolphilia on 2022/12/23.
//

#include "util_dir.h"

char * get_exe_path(){
#ifdef __WINDOWS__
    TCHAR path[MAX_PATH];   //パス取得用
    if (::GetModuleFileName(NULL, path, MAX_PATH))    //実行ファイルのフルパスを取得
    {   //取得に成功
        TCHAR* ptmp = _tcsrchr(path, _T('\\')); // \の最後の出現位置を取得
        if (ptmp != NULL)
        {   //ファイル名を削除
            ptmp = _tcsinc(ptmp);   //一文字進める
            *ptmp = _T('\0');
        }
        else
        {
            //エラー：\が無い
        }
    }
    else
    {
       //エラー：取得に失敗
    }
#endif
#ifdef __LINUX__
    char path[PATH_MAX + 1];

    const size_t LINKSIZE = 100;
    char link[LINKSIZE];

    // 実行中のプロセスのシンボリックリンク
    snprintf(link, LINKSIZE, "/proc/%d/exe", getpid() );

    // ファイルパスが得る
    readlink( link, path, PATH_MAX);
#endif

#ifdef __MACOS__
    uint32_t bufsize = PATH_MAX;
    char path[PATH_MAX];
    _NSGetExecutablePath(path, &bufsize);
#endif
    return path;
}

char *get_cur_path() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        return cwd;
    } else {
        return NULL;
    }
}