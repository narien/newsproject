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

//important req: "Changes to the database are immediately reflected on disk."

DatabaseDisk::DatabaseDisk() {

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
	ngfolder.append(to_string(readNewsgroupCntr()));

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

//return the newsgroup counter and increment with one
int DatabaseDisk::readNewsgroupCntr() {

	int cntr;
	string db = path;
	db.append(".db.txt");

	if(ifstream(db)) {
		fstream fs(db);
		if(fs.is_open())
		{
			fs >> cntr;
			cntr++;
			fs.seekp(0, std::ios::beg);
			fs.clear();
			fs << to_string(cntr);
		    fs.close();
		}
		else {
			printf("Unable to open .db.txt file.\n");
		}
	}
	else {
		//create file
		ofstream ofs(db);
		if(!ofs) {
			printf("File .db.txt could not be created.\n");
		}
		//start with 1
		ofs << "1";
		ofs.close();
	}
	return cntr-1; //return original value
}

//return the article counter and increment with one
int DatabaseDisk::readArticleCntr() {

	return 0;
}