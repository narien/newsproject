//Database on disk

#include "databasedisk.h"

#include <sys/stat.h>
#include <fstream>

/*---File Structure---
db (root)
	1 (dir name = newsgroup id)
		newsgroup.txt (2 lines(name of newsgroup, article counter))
		1.txt (name = article id, 3 lines(title, author, text))
	.db.txt (newsgroup counter)
---*/

DatabaseDisk::DatabaseDisk() {

	newsgroup_cntr = 1;

	//check if dir exists
	struct stat sb;
	if (!(stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))) {
		//create dir in current directory with no restrictions
		mkdir(path.c_str(), 0777);
	}
}

DatabaseDisk::~DatabaseDisk() {

}

bool DatabaseDisk::insertNewsgroup(string& title) {

	string ngfolder = path;
	ngfolder += to_string(newsgroup_cntr);
	ngfolder += title;

	//have to save this somehow for later...
	newsgroup_cntr++;

	//check if dir exists
	struct stat sb;
	if (!(stat(ngfolder.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))) {
		mkdir(ngfolder.c_str(), 0777);
		return true;
	}
	else {
		return false;
	}
}

bool DatabaseDisk::insertArticle(int& newsgroup_id, string& article_title, string& article_author, string& article_text) {

	//check if article exists as regular file
/*	struct stat sb;
	if (!(stat(ngfolder.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))) {
		mkdir(ngfolder.c_str(), 0777);
	}*/

	return false;
}

bool DatabaseDisk::removeNewsgroup(int& newsgroup_id) {

	return false;
}

//returns 1 if it worked, 0 if no such newsgroup and -1 if no such article
int DatabaseDisk::removeArticle(int& newsgroup_id, int& article_id) {

	return 0;
}

vector<pair<int, string>> DatabaseDisk::listNewsgroups() {

	vector<pair<int, string>> v;
	return v;
}

//returns 1 if it worked, 0 if no such newsgroup
bool DatabaseDisk::listArticles(int& newsgroup_id, vector<pair<int, string>>& articles) {

	return false;
}

//returns 1 if it worked, 0 if no such newsgroup and -1 if no such article, writes the relevant data to the input variables
int DatabaseDisk::getArticle(const int& newsgroup_id, const int& article_id, string& article_title, string& article_author, string& article_text) {

	return 0;
}

/*void DatabaseDisk::writeNewsgroupCntr() {

	string db = path;
	db.append(".db.txt");

	ofstream fout (db);
	if (fout.is_open())
	{
	    fout << "This is a line.\n";
	    fout.close();
	}
	else {
		printf("Unable to open database file.");
	}
}*/

//readcounters at the same time?
