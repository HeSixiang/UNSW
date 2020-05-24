import preprocess
import numpy as np
import pandas as pd
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer
import time
from collections import Counter
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, classification_report
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.model_selection import train_test_split

# Reading the data from file
file_train_data = preprocess.read_file("training.csv")
file_test_data = preprocess.read_file("test.csv")

# get rid of first line
file_train_data = file_train_data[1:]
file_test_data = file_test_data[1:]

# must do
preprocess.clean_data_isalpha(file_train_data)
preprocess.clean_data_isalpha(file_test_data)

######### Balance the data set #########
#tag, info_dic_train = preprocess.clean_data_helper_get_info(file_train_data)
#file_train_data = preprocess.clean_data_balance(file_train_data, tag)
######### Balance the data set #########

# store the data into seprate array
article_number,text_data,article_topic = preprocess.process_data(file_train_data)
test_number, test_data, test_topic = preprocess.process_data(file_test_data)

(unique, counts) = np.unique(article_topic, return_counts=True)
frequencies = np.asarray((unique, counts)).T
print("The frequency of different articles types (training set)")
print(frequencies)

(unique, counts) = np.unique(test_topic, return_counts=True)
frequencies = np.asarray((unique, counts)).T
print("The frequency of different articles types (testing set)")
print(frequencies)
print()

# start to preprocess
train_data = np.array(text_data)
test_data = np.array(test_data)

# CountVectorizer
# CountVectorizer just counts the word frequencies.
count = CountVectorizer()
bag_of_words = count.fit_transform(train_data)
test_bag = count.transform(test_data)

# TfidfVectorizer
# TfidfVectorizer considers overall document weightage of a word.
tfid = TfidfVectorizer(sublinear_tf=True)
tfid_of_words = tfid.fit_transform(train_data)
test_tfid = tfid.transform(test_data)


################## Put you model here ###################
clf = GradientBoostingClassifier(learning_rate=0.075 , n_estimators=100, max_depth=13)
################## Put you model here ###################

# Cross Vaildation (lazy version)
# For CountVectorizer
random_state_list = [42, 5, 23]
accuracy_score_list = []
for random_state in random_state_list:
    x_train, x_develop, y_train, y_develop = train_test_split(bag_of_words, article_topic, train_size = 9000, test_size = 500, random_state = random_state)
    model = clf.fit(x_train, y_train)
    y_pred = model.predict(x_develop)
    print("The reprot for training data set(random_state =",  random_state, "), CountVectorizer")
    print(classification_report(y_develop, y_pred))
    accuracy_rate = model.score(x_develop, y_develop)
    accuracy_score_list.append(accuracy_rate)
    print("The accuracy score: ", accuracy_rate)
print("accuracy_score_list is ", accuracy_score_list)
print()

accuracy_score_list = []
# For TfidfVectorizer
for random_state in random_state_list:
    x_train, x_develop, y_train, y_develop = train_test_split(tfid_of_words, article_topic, train_size = 9000, test_size = 500, random_state = random_state)
    model = clf.fit(x_train, y_train)
    y_pred = model.predict(x_develop)
    print("The reprot for training data set(random_state =",  random_state, "), CountVectorizer")
    print(classification_report(y_develop, y_pred))
    accuracy_rate = model.score(x_develop, y_develop)
    accuracy_score_list.append(accuracy_rate)
    print("The accuracy score: ", accuracy_rate)
print("accuracy_score_list is ", accuracy_score_list)
print()

# For CountVectorizer 
counter_model = clf.fit(bag_of_words, article_topic)
y_pred = counter_model.predict(test_bag)
print("The reprot for testing data set, CountVectorizer")
print(classification_report(test_topic, y_pred))
print()

predict_result = counter_model.predict_proba(test_bag)
topic_list = ["ARTS CULTURE ENTERTAINMENT", "BIOGRAPHIES PERSONALITIES PEOPLE", "DEFENCE", "DOMESTIC MARKETS", "FOREX MARKETS", "HEALTH", "IRRELEVANT", "MONEY MARKETS", "SCIENCE AND TECHNOLOGY", "SHARE LISTINGS", "SPORTS"]
for i in range(11):
    temp_list = []
    for e in predict_result: temp_list.append(e[i])
    top_10_idx = np.argsort(temp_list)[-10:]
    print(topic_list[i])
    print(top_10_idx)
    for e in top_10_idx: print(test_topic[e], "     score=", temp_list[e], "    index=", e+9501)
    print()

# For TfidfVectorizer
tfid_model = clf.fit(tfid_of_words, article_topic)
y_pred = tfid_model.predict(test_tfid)
print("The reprot for testing data set, CountVectorizer")
print(classification_report(test_topic, y_pred))
print()


predict_result = tfid_model.predict_proba(test_bag)
topic_list = ["ARTS CULTURE ENTERTAINMENT", "BIOGRAPHIES PERSONALITIES PEOPLE", "DEFENCE", "DOMESTIC MARKETS", "FOREX MARKETS", "HEALTH", "IRRELEVANT", "MONEY MARKETS", "SCIENCE AND TECHNOLOGY", "SHARE LISTINGS", "SPORTS"]
for i in range(11):
    temp_list = []
    for e in predict_result: temp_list.append(e[i])
    top_10_idx = np.argsort(temp_list)[-10:]
    print(topic_list[i])
    print(top_10_idx)
    for e in top_10_idx: print(test_topic[e], "     score=", temp_list[e], "    index=", e+9501)
    print()
