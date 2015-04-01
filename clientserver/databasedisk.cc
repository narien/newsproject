//Database on disk

#include "databasedisk.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <limits>

/*---File Structure---
db (root)
	1 (dir name = newsgroup id)
		newsgroup.txt (2 lines(name of newsgroup, article counter))
		1.txt (name = article id, 3 lines(title, author, text))
	.db.txt (newsgroup counter)
---*/

//req: "Changes to the database are immediately reflected on disk."
//req: no limits on titles ,authors and texts

//title and author on one line only
//text can be multiline

DatabaseDisk::DatabaseDisk() {

	//check if dir exists
	DIR* dir = opendir(path.c_str());
	if(!dir) {
		//create dir in current directory with no restrictions
		mkdir(path.c_str(), 0777);
	}
	closedir(dir);
}

DatabaseDisk::~DatabaseDisk() {

}

bool DatabaseDisk::insertNewsgroup(string& title) {

	//check if newsgroup title exists
	for (auto i : listNewsgroups()) {
		//newsgroup title already exists, return false
		if(i.second == title) {
		return false;
		}
	}

	//create new newsgroup directory
	string ngfolder = path;
	ngfolder.append(to_string(readNewsgroupCntr()));
	string ngfile = ngfolder;
	ngfile.append("/newsgroup.txt");

	DIR* dir = opendir(ngfolder.c_str());
	if(!dir) {
		mkdir(ngfolder.c_str(), 0777);
	}
	else {
		//om databasen är intakt borde man aldrig hamna här
		cout << "Critical database error!" << endl;
	}
	closedir(dir);

	//create file
	ofstream ofs(ngfile);
	if(ofs.is_open()) {
		ofs << title << '\n';
		//article counter start with 1
		ofs << "1\n";
		ofs.close();
		return true;
	}
	else {
		cout << "File " << ngfile << " could not be created." << endl;
	}
	return false;
}

bool DatabaseDisk::insertArticle(int& newsgroup_id, string& article_title, string& article_author, string& article_text) {

	string art_path = path;
	art_path.append(to_string(newsgroup_id));
	art_path.append("/");

	//does newsgroup exist?
	DIR* dir = opendir(art_path.c_str());
	if(!dir) {
		//no such newsgroup
		closedir(dir);
		return false;
	}
	closedir(dir);

	art_path.append(to_string(readArticleCntr(newsgroup_id)));
	art_path.append(".txt");

	//check if article exists
	if(!ifstream(art_path)) {
		ofstream os(art_path);
		if(os.is_open()) {
			os << article_title << '\n' << article_author << '\n';
			//text can be multiline
			os.write(article_text.c_str(), article_text.length());
			os << '\n';
			os.close();
			return true;
		}
	}
	else {
		//article exists, shouldnt happen
		cout << "Critical database error!" << endl;
	}
	return false;
}

bool DatabaseDisk::removeNewsgroup(int& newsgroup_id) {

	string ng_path = path;
	ng_path.append(to_string(newsgroup_id));
	ng_path.append("/");

	//does newsgroup exist?
	DIR* dir = opendir(ng_path.c_str());
	if(dir) {
		struct dirent* entry;
		while((entry = readdir(dir)) != NULL) {

			//all files in newsgroup dir
			if(entry->d_type == isFile && entry->d_name[0] != '.') {

				string local_path = ng_path;
		    	local_path.append(entry->d_name);

		    	//remove all files in newsgroup directory so that the directory itself can be deleted
				if(remove(local_path.c_str()) != 0) {
					cout << local_path << " cannot be removed." << endl;
				}
			}
		}
	}
	else {
		//no such newsgroup
		closedir(dir);
		return false;
	}
	closedir(dir);

	//remove directory
	if(rmdir(ng_path.c_str()) == 0) {
		return true;
		}
	else {
		cout << "Newsgroup with id: " << newsgroup_id << " cannot be removed." << endl;
	}
	return false;
}

//returns 1 if it worked, 0 if no such newsgroup and -1 if no such article
int DatabaseDisk::removeArticle(int& newsgroup_id, int& article_id) {

	string art_path = path;
	art_path.append(to_string(newsgroup_id));
	art_path.append("/");

	//does newsgroup exist?
	DIR* dir = opendir(art_path.c_str());
	if(!dir) {
		//no such newsgroup
		closedir(dir);
		return 0;
	}
	closedir(dir);

	art_path.append(to_string(article_id));
	art_path.append(".txt");

	//check if article exists
	if(ifstream(art_path)) {
		if(remove(art_path.c_str()) == 0) {
			return 1;
		}
		else {
			cout << "Article with id: " << article_id << " cannot be removed." << endl;
		}
	}
	else {
		//article doesnt exists
		return -1;
	}
	return 0;
}

vector<pair<int, string>> DatabaseDisk::listNewsgroups() {

	vector<pair<int, string>> v;

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

						v.push_back(make_pair(atoi(entry->d_name), tmp_ngname));
					}
					else {
						cout << "Unable to open " << local_path << " file." << endl;
					}
				}
				else {
					cout << "Newsgroup data file does not exist." << endl;
				}
			}
		}
	}
	else {
		cout << "Unable to open " << path << " directory." << endl;
	}
	closedir(dir);
	return v;
}

//returns 1 if it worked, 0 if no such newsgroup
bool DatabaseDisk::listArticles(int& newsgroup_id, vector<pair<int, string>>& articles) {

	string art_path = path;
	art_path.append(to_string(newsgroup_id));
	art_path.append("/");

	//does newsgroup exist?
	DIR* dir = opendir(art_path.c_str());
	if(dir) {
		struct dirent* entry;
		while((entry = readdir(dir)) != NULL) {

			//all files in newsgroup dir exept newsgroup.txt ie all articles
			if(entry->d_type == isFile && entry->d_name[0] != '.' && entry->d_name[0] != 'n') {

				string local_path = art_path;
		    	local_path.append(entry->d_name);

		    	//check if article exists
				if(ifstream(local_path)) {
					ifstream fs(local_path);
					if(fs.is_open()) {
						string tmp_aname;
						fs >> tmp_aname;
						fs.close();

						//get article id from file name
						string tmp_aid = entry->d_name;
						int pos = tmp_aid.find(".txt");
						string tmp_aaid = tmp_aid.substr(0, pos);

						articles.push_back(make_pair(atoi(tmp_aaid.c_str()), tmp_aname));
					}
					else {
						cout << "Unable to open " << local_path << " file." << endl;
					}
				}
			}
		}
	}
	else {
		//no such newsgroup
		closedir(dir);
		return false;
	}
	closedir(dir);
	return true;
}

//returns 1 if it worked, 0 if no such newsgroup and -1 if no such article, writes the relevant data to the input variables
int DatabaseDisk::getArticle(const int& newsgroup_id, const int& article_id, string& article_title, string& article_author, string& article_text) {

	string art_path = path;
	art_path.append(to_string(newsgroup_id));
	art_path.append("/");

	//does newsgroup exist?
	DIR* dir = opendir(art_path.c_str());
	if(!dir) {
		//no such newsgroup
		closedir(dir);
		return 0;
	}
	closedir(dir);

	art_path.append(to_string(article_id));
	art_path.append(".txt");

	//check if article exists
	if(ifstream(art_path)) {
		ifstream fs(art_path);
		if(fs.is_open()) {
			fs >> article_title;
			fs >> article_author;

			//read text
			string tmp_line;
			while(getline(fs, tmp_line) ) {
				if(!tmp_line.empty()) {
			      	article_text.append(tmp_line);
			      	article_text.append("\n");
			  	}
			}
			fs.close();
			//remove last newline
			article_text.erase(article_text.end()-1, article_text.end());
			return 1;
		}
	}
	else {
		//article doesnt exists
		return -1;
	}
	return 0;
}

//return the newsgroup counter and increment with one
int DatabaseDisk::readNewsgroupCntr() {

	int cntr = -1;
	string db = path;
	db.append(".db.txt");

	if(ifstream(db)) {
		fstream fs(db);
		if(fs.is_open()) {
			fs >> cntr;
			fs.seekp(0, std::ios::beg);
			fs.clear();
			cntr++;
			fs << to_string(cntr) << '\n';
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
		//start with 1 and add 1
		ofs << "2\n";
		ofs.close();
		return 1;
		}
		else {
			cout << "File " << db << " could not be created." << endl;
		}
	}
	return --cntr;
}

//return the article counter and increment with one
int DatabaseDisk::readArticleCntr(int& newsgroup_id) {

	int cntr = -1;
	string ng = path;
	ng.append(to_string(newsgroup_id));
	ng.append("/newsgroup.txt");

	if(ifstream(ng)) {
		fstream fs(ng);
		if(fs.is_open()) {
			//ignore first line (newsgroup title)
			fs.ignore(numeric_limits<streamsize>::max(), '\n');
			long pos = fs.tellp();
			fs >> cntr;
			fs.seekp(pos, std::ios::beg);
			fs.clear();
			cntr++;
			fs << to_string(cntr) << '\n';
		    fs.close();
		}
		else {
			cout << "Unable to open " << ng << " file." << endl;
		}
	}
	else {
		//om databasen är intakt borde man aldrig hamna här
		cout << "Critical database error!" << endl;
	}
	return --cntr;
}