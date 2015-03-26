//Database in memory

#include "databasememory.h"

#include <algorithm>


DatabaseMemory::DatabaseMemory() {

	newsgrp_cntr = 1;
}

bool DatabaseMemory::insertNewsgroup(string& title) {

	//check if already exist
	auto it = find_if(newsgroups.begin(), newsgroups.end(), [title] (const pair<int, Newsgroup>& m) { return m.second.title == title; });

	if (!title.empty() && it == newsgroups.end()) {

		Newsgroup tmp_grp = Newsgroup();
		tmp_grp.title = title;
		newsgroups.insert({newsgrp_cntr, tmp_grp});

		newsgrp_cntr++;
		return true;
	}

	else {
		return false;
	}
}

bool DatabaseMemory::insertArticle(int& newsgroup_id, string& article_title, string& article_author, string& article_text) {

	if (newsgroup_id>0 && !article_title.empty()) {

		Article tmp_article = {article_title, article_author, article_text};
		newsgroups[newsgroup_id].articles.insert({newsgroups[newsgroup_id].article_cntr, tmp_article});

		newsgroups[newsgroup_id].article_cntr++;
		return true;
	}

	else {
		return false;
	}
}


bool DatabaseMemory::removeNewsgroup(int& newsgroup_id) {

	auto it = newsgroups.find(newsgroup_id);

	if(it != newsgroups.end()) {
		newsgroups.erase(it);
        return true;
	}
	else {
		return false;
	}
}

int DatabaseMemory::removeArticle(int& newsgroup_id, int& article_id) {

	auto it = newsgroups.find(newsgroup_id);

	if(it != newsgroups.end()) {
		/**todo**/
        if(true){
            return 1;
        } else {
            return -1;
        }
	}
	else {
		return 0;
	}
}


DatabaseMemory::~DatabaseMemory() {

}

int main() {

}