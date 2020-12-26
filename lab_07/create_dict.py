from pymorphy2 import *
morph = MorphAnalyzer()

f = open("dictionary", 'r', encoding='utf-8')

normal_words = []
for word in f.readlines():
    word = word.replace('\n','')
    normal_words.append(morph.parse(word)[0].normal_form)

normal_words = list(set(normal_words))
normal_words.sort()

f.close()
f = open("dictionary_normal", 'w', encoding='utf-8')
for word in normal_words:
    f.write(word + '\n')

# words = set()

# for s in f.readlines():
#     if s.find("/section") > 0:
#         continue

#     s = s.replace("<p>", '').replace("</p>", '').replace("</title>",'').replace("\n", '')
#     for word in s.split(" "):
#         word = word.replace(",", '').replace(".", '').replace("!", '').replace("…", '').replace(":", '').replace(";", '').replace("?", '')
#         word = word.replace("«", '').replace("»", '').replace("\xa0–", '').replace('(', '').replace(')', '').replace('“', '')
#         words.add(word.lower())

# res = open("dictionary", 'w', encoding='utf-8')
# words = list(words)
# words.sort()
# for word in words:
#     res.write(word+'\n')

