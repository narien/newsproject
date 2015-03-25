//Database in memory

#include "databasememory.h"

#include <algorithm>


DatabaseMemory::DatabaseMemory() {

	newsgrp_cntr = 1;
}

bool DatabaseMemory::insert_newsgroup(string& title) {

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

bool DatabaseMemory::insert_article(int& newsgroup_id, string& article_title, string& article_author, string& article_text) {

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


bool DatabaseMemory::remove_newsgroup(int& newsgroup_id) {

	auto it = newsgroups.find(newsgroup_id);

	if(it != newsgroups.end()) {
		newsgroups.erase(it);
	}
	else {
		return false;
	}
}

bool DatabaseMemory::remove_article(int& newsgroup_id, int& article_id) {

	auto it = newsgroups.find(newsgroup_id);

	if(it != newsgroups.end()) {
		
	}
	else {
		return false;
	}
}


DatabaseMemory::~DatabaseMemory() {

}

int main() {

}