/************************************************************
ディレクトリの読み込み
	http://www.syuhitu.org/other/dir.html
************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h> 
#include <dirent.h>
#include <string>

#include <FindFile.h>

using namespace std;

/************************************************************
************************************************************/

/******************************
******************************/
void FIND_FILE::print_cnt( int cnt )
{
	int i;
	for ( i = 0; i < cnt; i++ ){
		putchar( '\t' );
	}
}

/******************************
******************************/
void FIND_FILE::outputFileName(const string dirname, int cnt)
{
	DIR *pDir;
	struct dirent *pEnt;
	struct stat wStat;
	string wPathName;

	// ディレクトリを開く
	pDir = opendir( dirname.c_str() );
	if ( NULL == pDir ) {
		printf( "Error : in %s directoryt\n", dirname.c_str() );
		return ;
	}

	pEnt = readdir( pDir );
	while ( pEnt ) {
		// .と..は処理しない
		if ( strcmp( pEnt->d_name, "." ) && strcmp( pEnt->d_name, ".." ) ) {
		
			wPathName = dirname + "/" + pEnt->d_name;
			
			// ファイルの情報を取得
			if ( stat( wPathName.c_str(), &wStat ) ) {
				printf( "Failed to get stat %s \n", wPathName.c_str() );
				break;
			}
			
			if ( S_ISDIR( wStat.st_mode ) ) {
				// ディレクトリの場合
				print_cnt( cnt );
				printf( "Directory:%s\n", pEnt->d_name );
				outputFileName( wPathName, cnt + 1 );
				
			} else {
				// ファイルの場合
				print_cnt( cnt );
				printf( "%s\n", pEnt->d_name );
				
				print_cnt( cnt + 1 );
				vector<string> str = ofSplitString(pEnt->d_name, ".");
				printf("ext = %s\n", str[str.size()-1].c_str());
				
				if(str[str.size()-1] == "txt"){
					print_cnt( cnt + 1 );
					vector<string> str2 = ofSplitString(pEnt->d_name, "#");
					if(str2.size() == 3){
						printf( "bpm = %d\n", atoi(str2[1].c_str()) );
					}else{
						printf( "DefaultBpm = 100\n");
					}
				}
			}
		}
		
		// 次のファイルを検索する
		pEnt = readdir( pDir );
	}

	// ハンドルを閉じる
	closedir( pDir );
}

