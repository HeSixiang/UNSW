# Written by Sixiang He
# Apr 2020

import csv
import numpy as np
import pandas as pd
import random

from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer

import enchant
from collections import Counter

#read file
def read_file(file):
    with open(file) as file:
        reader = csv.reader(file)
        data = [rows for rows in reader]
    return data

# This function will remove all non-alpha words and store back
# Note: this function will change your parameter data
# Parameters: 
#   data(list->list): processed data
#   text_index(int): the text index for data list, default: 1 
# Returns:
#   delete_text_dic(dic): dictionary(key: deleted word, value: num of that word)
def clean_data_isalpha(data, text_index = 1):
    delete_text_dic = {}
    for d in data:
        text = d[text_index]
        text_list = text.split(",")
        if len(text_list) == 0: continue
        new_text = ""
        for t in text_list:
            temp = t.strip() # trim
            if  temp.isalpha()== True:
                new_text += "," + temp
            else:
                if temp in delete_text_dic:
                    delete_text_dic[temp] += 1
                else:
                    delete_text_dic[temp] = 1
        d[text_index] = new_text[1:] # get rid of first ','
    return delete_text_dic

# This function will try to fix all the word and store back
# If it can not fix, it will just put it back
# Note: this function will change your parameter data
# Parameters: 
#   data(list->list): processed data
#   text_index(int): the text index for data list, default: 1 
# Returns:
#   change_word_dic(dic): dictionary(key: orginal word, value: fix word)
def clean_data_fix_word(data, text_index = 1):
    change_word_dic= {}
    check_dic = enchant.Dict("en_US")
    for d in data:
        # print("working on ", d[0])
        text = d[text_index]
        text_list = text.split(',')
        if len(text_list) == 0: 
            print("No text? might be something wrong")
            continue
        new_text = ""
        for t in text_list:
            if not check_dic.check(t):
                if t in list(change_word_dic.keys()):
                    new_text += "," + change_word_dic[t]
                    continue
                suggest = check_dic.suggest(t)
                num_of_suggest = len(suggest)
                # only care top 5
                if num_of_suggest > 5: num_of_suggest = 5
                find_ok_suggest_word = False
                if (t + 'e') in suggest:
                    new_text += "," + t + 'e'
                    continue
                for i in range(num_of_suggest):
                    suggest_word = suggest[i]
                    if not suggest_word.isalpha(): continue
                    if not suggest_word.islower(): continue
                    if len(suggest_word) < len(t): continue
                    if len(suggest_word) > len(t) + 2: continue
                    t_char_list = []
                    for c in t: t_char_list.append(c)
                    suggest_char_list = []
                    for c in suggest_word: suggest_char_list.append(c)
                    intersetion = []
                    for c in t: intersetion.append(c)
                    for c in suggest_word: intersetion.append(c)
                    intersetion = list(set(intersetion))
                    a = len(t_char_list)
                    b = len(suggest_char_list)
                    c = len(intersetion)
                    if (a + b - c) > 4: continue
                    find_ok_suggest_word = True 
                    change_word_dic[t] = suggest_word
                    break
                if not find_ok_suggest_word: suggest_word = t
                new_text += "," + suggest_word
            else:
                # is valid word
                new_text += "," + t
        d[text_index] = new_text[1:]
    return change_word_dic
    
# This function is helper method for cleaning data.
# Parameters:
#   data(list->list): processed data
#   text_index(int): the text index for data list, default: 1 
#   tag_index(int): the tag index for data list, default: 2 
# Returns:
#   tag(list): distinct tag, should be 11 in our project
#   info_dic(dic->dic->int): dictionary(key: tag, value: dictionary2)
#                           dictionary2(key: word, value: num of word)
def clean_data_helper_get_info(data, text_index = 1, tage_index = 2):
    info_dic = {}
    tag = []
    for d in data: tag.append(d[tage_index])
    tag = list(set(tag))
    for t in tag: info_dic[t] = {}
    for d in data:
        text = d[text_index]
        text_list = text.split(",")
        for t in text_list:
            if len(t) == 0: continue  # skip ''
            if t in info_dic[d[tage_index]]:
                info_dic[d[tage_index]][t] += 1
            else:
                info_dic[d[tage_index]][t] = 1
    return tag, info_dic

# This function will remove all the duplicate elements that appeared 
# in all type of artcles based on input parameters.
# Parameters:
#   tag(list): distinct tag, should be 11 in our project
#   info_dic(dic->dic->int): dictionary(key: tag, value: dictionary2)
#                           dictionary2(key: word, value: num of word)
# Returns:
#   Diffenrent between new and old is new does not contains the duplicated word 
#   new_info_dic(dic->dic->int): dictionary(key: tag, value: dictionary2)
#                               dictionary2(key: word, value: num of word)
#   all_tag_contain_list(list): the list of word that all tag contained 
def clean_data_duplicate(tag, info_dic):
    all_tag_contain_list = []
    for i in tag:
        current = list(info_dic[i].keys())
        #print(i, "  ", len(current))
        if len(all_tag_contain_list) == 0:
            all_tag_contain_list = current
        else:
            temp_list = []
            for dup_key in all_tag_contain_list:
                if dup_key not in current:
                    temp_list.append(dup_key)
            for delete_key in temp_list:
                all_tag_contain_list.remove(delete_key)
    #print(len(all_tag_contain_list))
    new_info_dic = info_dic.copy()
    for i in tag:
        curren_dic = new_info_dic[i]
        for dup in all_tag_contain_list:
            curren_dic.pop(dup)
    return new_info_dic, all_tag_contain_list

# This function will delete all minor word based on you parameter
# The threshold is factor to determind which word is minor
# You can pass differne threshold for different tag
# Parameters:
#   tag(list): distinct tag, should be 11 in our project
#   info_dic(dic->dic->int): dictionary(key: tag, value: dictionary2)
#                           dictionary2(key: word, value: num of word)
#   min_dic(dic): dictionary(key: tag, value: threshold)
# Returns:
#   Diffenrent between new and old is new does not contains minor defined
#   new_info_dic(dic->dic->int): dictionary(key: tag, value: dictionary2)
#                               dictionary2(key: word, value: num of word)
def clean_data_minor(tag, info_dic, min_list = {}):
    if len(min_list) == 0: 
        for i in tag: min_list[i] = 1 # set up default value
    new_info_dic = info_dic.copy()
    for i in tag:
        curren_dic = new_info_dic[i]
        curren_dic_key_list = list(curren_dic.keys())
        delete_key = []
        for key in curren_dic_key_list:
            if curren_dic[key] <= min_list[i]:
                delete_key.append(key)
        for key in delete_key:
            curren_dic.pop(key)
    return new_info_dic

# This function will return dic with feature word for different tags basd on info_dic
# info_dic should be the final dic you want
# Parameters:
#   tag(list): distinct tag, should be 11 in our project
#   info_dic(dic->dic->int): dictionary(key: tag, value: dictionary2)
#                           dictionary2(key: word, value: num of word)
#   top(dic): dictionary((key: tag, value: num of top word keeped)
#   The default value of top(dic) is to keep all the word.
# Returns:
#   keep_word_dic(dic->list): dictionary(key: tag, value: list of word)              
def clean_data_top(tag, info_dic, top_dic = {}):
    keep_word_dic = {}
    if len(top_dic) == 0:
        for i in tag: keep_word_dic[i] = list(info_dic[i].keys())
    else:
        for i in tag:
            keep_word_dic[i] = []
            curren_dic = info_dic[i]
            d = Counter(curren_dic)
            for k, v in d.most_common(top_dic[i]):
                keep_word_dic[i].append(k)
    return keep_word_dic 

# This function is the final step of cleanning data
# It will only keep the data based on keep_word_dic
# Note: this function will change your parameter data
# Parameters: 
#   data(list->list): processed data
#   keep_word_dic(dic->list): dictionary(key: tag, value: list of word)
#   text_index(int): the text index for data list, default: 1 
#   tag_index(int): the tag index for data list, default: 2   
# Returns:
#   no return value, it will change the parameter data 
def clean_data_final(data, keep_word_dic, text_index = 1, tag_index = 2):
    for d in data:
        text = d[text_index]
        text_list = text.split(",")
        if len(text_list) == 0: continue
        new_text = ""
        for t in text_list:
            # print(d[tag_index])
            if t in keep_word_dic[d[tag_index]]:
                new_text += "," + t
        if len(new_text) == 0:
            print("Warning: This is some data empty after cleanning")
            d[text_index] = ''
        else:
            d[text_index] = new_text[1:]



def clean_data_balance(data, tag, text_index = 1, tag_index = 2):
    data_dic = {}
    for t in tag: data_dic[t] = []
    for d in data: data_dic[d[tag_index]].append(d)
    new_data = []
    for t in tag:
        for i in range(1000): 
            n = random.randint(0,len(data_dic[t]) - 1)
            new_data.append(data_dic[t][n]) 
    return new_data

def process_data(data):
    article_number = []
    text_data = []
    article_topic = []
    for i in data:
        #print(i)
        article_number.append(i[0])
        text_data.append(i[1])
        article_topic.append(i[2])
    return article_number,text_data,article_topic

'''
if t == "ARTS CULTURE ENTERTAINMENT":
    # 117 * 5
    for i in range(5): new_data += data_dic[t]
elif t == "BIOGRAPHIES PERSONALITIES PEOPLE":
    # 167 * 3
    for i in range(3): new_data += data_dic[t]
elif t == "DEFENCE":
    # 258 *2
    for i in range(2): new_data += data_dic[t]
elif t == "DOMESTIC MARKETS":
    # 133 * 4
    for i in range(4): new_data += data_dic[t]
elif t == "HEALTH":
    # 183 * 3
    for i in range(3): new_data += data_dic[t]
elif t == "SCIENCE AND TECHNOLOGY":
    # 70 * 7
    for i in range(7): new_data += data_dic[t]
elif t  == "SHARE LISTINGS":
    # 218 * 3
    for i in range(3): new_data += data_dic[t]
elif t == "IRRELEVANT":
'''
     
        
