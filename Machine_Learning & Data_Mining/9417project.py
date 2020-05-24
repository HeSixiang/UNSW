import csv
import numpy as np
import pandas as pd

from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer

from sklearn.naive_bayes import MultinomialNB
from sklearn.naive_bayes import BernoulliNB
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier

from sklearn.ensemble import GradientBoostingClassifier
from sklearn.ensemble import AdaBoostClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import BaggingClassifier

from sklearn.metrics import classification_report, confusion_matrix

import time

#from xgboost import XGBClassifier

from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, classification_report

#read file
def read_file(file):
    with open(file) as file:
        reader = csv.reader(file)
        data = [rows for rows in reader]
    return data

# process the data, store the relative index into differnet arrays
def process_data(data):
    article_number = []
    text_data = []
    article_topic = []
    for i in data[1:]:
        article_number.append(i[0])
        text_data.append(i[1])
        article_topic.append(i[2])
    return article_number,text_data,article_topic

def clean_data(data):
    result = []
    for article_words in data:
        result.append(article_words.replace(",", " "))
    return result

if __name__ == '__main__':

    # Reading the data from file
    file_train_data = read_file("training.csv")
    file_test_data = read_file("test.csv")

    # drop frist row and store the data into seprate array
    article_number,text_data,article_topic = process_data(file_train_data)
    test_number, test_data, test_topic = process_data(file_test_data)

    # clean the data
    # no null or missing data and all lower case in our dataset
    text_data = clean_data(text_data)
    test_data = clean_data(test_data)
 

    # Exploratory data analysis
    # primarily EDA is for seeing what the data can tell us beyond 
    # the formal modeling or hypothesis testing task.
 
    (unique, counts) = np.unique(article_topic, return_counts=True)
    frequencies = np.asarray((unique, counts)).T
    #print("The frequency of different articles types (training set)")
    #print(frequencies)

    (unique, counts) = np.unique(test_topic, return_counts=True)
    frequencies = np.asarray((unique, counts)).T
    #print("The frequency of different articles types (testing set)")
    #print(frequencies)

    # s = pd.Series(article_topic)
    # print(s.describe())
    # TODO: mignt be more?

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
    tfid = TfidfVectorizer()
    tfid_of_words = tfid.fit_transform(train_data)
    test_tfid = tfid.transform(test_data)

    # TODO?    
    # https://www.geeksforgeeks.org/python-word-embedding-using-word2vec/
    # https://towardsdatascience.com/word-embeddings-exploration-explanation-and-exploitation-with-code-in-python-5dac99d5d795
    
    # start to build model
    X_count = bag_of_words
    X_count_train = X_count[:9000]

    X_tfid = tfid_of_words
    X_tfid_train = X_tfid[:9000]

    X_count_test = test_bag
    
    X_tfid_test = test_tfid

    Y = np.array(article_topic)
    Y_train = Y[:9000]

    Y_test = np.array(test_topic)
    # TODO: development set

    # MultinomialNB
    # https://scikit-learn.org/stable/modules/generated/sklearn.naive_bayes.MultinomialNB.html
    start_time = time.time()
    print("-MultinomialNB-")
    clf = MultinomialNB()
    model = clf.fit(X_count_train,Y_train)
    predicted_y = model.predict(X_count_test)
    print("CountVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    model = clf.fit(X_tfid_train,Y_train)
    predicted_y = model.predict(X_tfid_test)
    print("TfidfVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    print("--- %s seconds ---" % (time.time() - start_time))

    # BernoulliNB
    # https://scikit-learn.org/stable/modules/generated/sklearn.naive_bayes.BernoulliNB.html
    start_time = time.time()
    print("-BernoulliNB-")
    clf = BernoulliNB()
    model = clf.fit(X_count_train,Y_train)
    predicted_y = model.predict(X_count_test)
    print("CountVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    model = clf.fit(X_tfid_train,Y_train)
    predicted_y = model.predict(X_tfid_test)
    print("TfidfVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    print("--- %s seconds ---" % (time.time() - start_time))

    # Support Vector Classifier
    # https://scikit-learn.org/stable/modules/generated/sklearn.svm.SVC.html
    start_time = time.time()
    print("-SupportVectorClassifier-")
    # clf = SVC(gamma='auto')
    clf = SVC(C=1.0, kernel='linear', degree=3, gamma='auto')
    model = clf.fit(X_count_train,Y_train)
    predicted_y = model.predict(X_count_test)
    print("CountVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    model = clf.fit(X_tfid_train,Y_train)
    predicted_y = model.predict(X_tfid_test)
    print("TfidfVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    print("--- %s seconds ---" % (time.time() - start_time))

    # DecisionTreeClassifier
    # https://scikit-learn.org/stable/modules/generated/sklearn.tree.DecisionTreeClassifier.html
    start_time = time.time()
    print("-DecisionTreeClassifier-")
    clf = DecisionTreeClassifier()
    model = clf.fit(X_count_train,Y_train)
    predicted_y = model.predict(X_count_test)
    print("CountVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    model = clf.fit(X_tfid_train,Y_train)
    predicted_y = model.predict(X_tfid_test)
    print("TfidfVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    print("--- %s seconds ---" % (time.time() - start_time))

    # NearestNeighbors
    # https://scikit-learn.org/stable/modules/generated/sklearn.neighbors.NearestNeighbors.html
    start_time = time.time()
    print("-NearestNeighbors-")
    clf = KNeighborsClassifier(n_neighbors=3)
    model = clf.fit(X_count_train,Y_train)
    predicted_y = model.predict(X_count_test)
    print("CountVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    model = clf.fit(X_tfid_train,Y_train)
    predicted_y = model.predict(X_tfid_test)
    print("TfidfVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    print("--- %s seconds ---" % (time.time() - start_time))

    # GradientBoostingClassifier
    # https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.GradientBoostingClassifier.html
    print("-GradientBoostingClassifier-")
    lr_list = [0.05, 0.075, 0.1, 0.25, 0.5, 0.75, 1]
    print("CountVectorizer -> ")
    for learning_rate in lr_list:
        start_time = time.time()
        #gb_clf = GradientBoostingClassifier(n_estimators=100, learning_rate=learning_rate,  max_depth=6, min_samples_leaf =1, max_features=1.0)
        gb_clf = GradientBoostingClassifier(learning_rate=learning_rate)
        model = gb_clf.fit(X_count_train, Y_train)
        predicted_y = model.predict(X_count_test)
        print("Learning rate: ", learning_rate, "accuracy_score: ", accuracy_score(predicted_y,Y_test))
        print("--- %s seconds ---" % (time.time() - start_time))
        #print("Accuracy score (training): {0:.3f}".format(gb_clf.score(X_count_train, Y_train)))
        #print("Accuracy score (validation): {0:.3f}".format(gb_clf.score(X_val, y_val)))
    
    print("TfidfVectorizer -> ")
    for learning_rate in lr_list:
        start_time = time.time()
        #gb_clf = GradientBoostingClassifier(n_estimators=100, learning_rate=learning_rate,  max_depth=6, min_samples_leaf =1, max_features=1.0)
        gb_clf = GradientBoostingClassifier(learning_rate=learning_rate)
        model = gb_clf.fit(X_tfid_train, Y_train)
        predicted_y = model.predict(X_tfid_test)
        print("Learning rate: ", learning_rate, "accuracy_score: ", accuracy_score(predicted_y,Y_test))
        print("--- %s seconds ---" % (time.time() - start_time))

    # AdaBoostClassifier
    # https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.AdaBoostClassifier.html
    start_time = time.time()
    print("-AdaBoostClassifier-")
    clf = AdaBoostClassifier()
    model = clf.fit(X_count_train,Y_train)
    predicted_y = model.predict(X_count_test)
    print("CountVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    model = clf.fit(X_tfid_train,Y_train)
    predicted_y = model.predict(X_tfid_test)
    print("TfidfVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    print("--- %s seconds ---" % (time.time() - start_time))

    # RandomForestClassifier
    # https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.RandomForestClassifier.html
    start_time = time.time()
    print("-RandomForestClassifier-")
    clf = RandomForestClassifier(n_estimators=200, max_depth=3, random_state=0)
    model = clf.fit(X_count_train,Y_train)
    predicted_y = model.predict(X_count_test)
    print("CountVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    model = clf.fit(X_tfid_train,Y_train)
    predicted_y = model.predict(X_tfid_test)
    print("TfidfVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    print("--- %s seconds ---" % (time.time() - start_time))

    # BaggingClassifier
    # https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.BaggingClassifier.html
    start_time = time.time()
    print("-BaggingClassifier-")
    clf = BaggingClassifier(base_estimator=SVC(), n_estimators=10, random_state=0)
    model = clf.fit(X_count_train,Y_train)
    predicted_y = model.predict(X_count_test)
    print("CountVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    model = clf.fit(X_tfid_train,Y_train)
    predicted_y = model.predict(X_tfid_test)
    print("TfidfVectorizer -> " + str(accuracy_score(predicted_y,Y_test)))
    print("--- %s seconds ---" % (time.time() - start_time))