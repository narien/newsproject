//Database on disk

#include "databasedisk.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>

/*---File Structure---
db (root)
	1 (dir name = newsgroup id)
		newsgroup.txt (2 lines(name of newsgroup, article counter))
		1.txt (name = article id, 3 lines(title, author, text))
	.db.txt (newsgroup counter)
---*/

//req: "Changes to the database are immediately reflected on disk."
//req: no limits on name

DatabaseDisk::DatabaseDisk() {

	//check if dir exists
	DIR* dir = opendir(path.c_str());
	if(!dir) {
		//create dir in current directory with no restrictions
		mkdir(path.c_str(), 0777);
	}
	closedir(dir);
	delete dir;
}

DatabaseDisk::~DatabaseDisk() {

}

bool DatabaseDisk::insertNewsgroup(string& title) {

	//check if newsgroup title exists
	DIR* dir = opendir(path.c_str());
	if(dir) {

		struct dirent* entry;
		while((entry = readdir(dir)) != NULL) {

			//all directories in db
			if(entry->d_type == isDir && entry->d_name[0] != '.') {
		    	string local_path = path;
		    	local_path.append(entry->d_name);
		    	local_path.append("/newsgroup.txt");

			    if(ifstream(local_path)) {
					fstream fs(local_path);

					if(fs.is_open()) {
						string tmp_ngname;
						fs >> tmp_ngname;
					    fs.close();

					    //newsgroup title already exists, return false
					    if(tmp_ngname == title) {
					    	return false;
					    }
					}
					else {
						cout << "Unable to open " << local_path << " file." << endl;
					}
				}
				else {
					cout << "Newsgroup file does not exist." << endl;
				}
			}
		}

	}
	else {
	cout << "Error opening " << path << " directory " << path << endl;
	}

	closedir(dir);
	delete dir;
	
	string ngfolder = path;
	ngfolder.append(to_string(readNewsgroupCntr())); //dont run every time
	string ngfile = ngfolder;
	ngfile.append("/newsgroup.txt");








	//check if dir exists
	struct stat sb;
	if(!(stat(ngfolder.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))) {
		mkdir(ngfolder.c_str(), 0777);

		//create file
		ofstream ofs(ngfile);
		if(ofs.is_open()) {
		ofs << title << '\n';
		//article counter start with 1
		ofs << "1";
		ofs.close();
		}
		else {
			cout << "File " << ngfile << " could not be created." << endl;
		}
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

	readArticleCntr();

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
			cout << "Unable to open " << db << " file." << endl;
		}
	}
	else {
		//create file
		ofstream ofs(db);
		if(ofs.is_open()) {
		//start with 1
		ofs << "1";
		ofs.close();
		}
		else {
			cout << "File " << db << " could not be created." << endl;
		}
	}
	return cntr-1;
}

//return the article counter and increment with one
int DatabaseDisk::readArticleCntr() {

	return 0;
}