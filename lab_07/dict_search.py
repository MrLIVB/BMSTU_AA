from time import perf_counter
from random import shuffle

name = 'dictionary_normal'
def readwords(filename):
    f = open(filename, 'r', encoding='utf-8')
    words = []
    for word in f.readlines():
        words.append(word.replace('\n',''))
    return words


words = readwords(name)

dictionary = []
for i in range(len(words)):
    dictionary.append([words[i], i])

def fullsearch(key, dictionary):
    for record in dictionary:
        if record[0] == key:
            return record
    return None

def binsearch(key, dictionary):
    lb = 0
    rb = len(dictionary) - 1
    while lb <= rb :
        mid = (lb + rb) // 2
        if dictionary[mid][0] == key:
            return dictionary[mid]
        elif dictionary[mid][0] < key:
            lb = mid + 1
        else:
            rb = mid - 1
    return None


def statseg(dictionary):
    segments = []
    for record in dictionary:
        added = False
        for i in range(len(segments)):
            if record[0][0] == segments[i][0][0]:
                segments[i][1].append(record)
                added = True
        if not added:
            segments.append([[record[0][0]], [record]])
    segments.sort(key=lambda record: len(record[1]), reverse=True)

    i = len(segments) - 1
    while i > 0:
        if len(segments[i][1]) < 100:
            for letter in segments[i][0]:
                segments[i - 1][0].append(letter)
            for word in segments[i][1]:
                segments[i - 1][1].append(word)
            segments.pop(i)
        i -= 1
    segments.sort(key=lambda record: len(record[1]), reverse=True)
    for segment in segments:
        segment[1].sort(key=lambda record: record[0])
    return segments


def segmentation(key, segments):
    for segment in segments:
        if key[0] in segment[0]:
            return binsearch(key, segment[1])

gl_seg = statseg(dictionary)
def test(key):
    print(fullsearch(key, dictionary))
    print(binsearch(key, dictionary))
    print(segmentation(key, gl_seg))

# test('ёкнуть')
# test('абажур')
# test('пролетать')
# test('Отсутствует')


REPEATS = 20
shuffle(words)

start = perf_counter()
for i in range(REPEATS):
    for word in words:
        fullsearch(word, dictionary)
elapsed_time = (perf_counter()-start) / REPEATS
print(f'Fullsearch={elapsed_time}')

start = perf_counter()
for i in range(REPEATS):
    for word in words:
        binsearch(word, dictionary)
elapsed_time = (perf_counter()-start) / REPEATS
print(f'Binsearch={elapsed_time}')

start = perf_counter()
for i in range(REPEATS):
    segmentated = statseg(dictionary)
    for word in words:
        segmentation(word, segmentated)
elapsed_time = (perf_counter()-start) / REPEATS
print(f'Segmentation={elapsed_time}')

max_time = -10
max_word =[]
min_time = 10
min_word =[]
for word in words:
    start = perf_counter()
    for i in range(200):
        binsearch(word, dictionary)
    elapsed_time = perf_counter() - start
    
    if max_time < elapsed_time:
        max_time = elapsed_time
        max_word = word
    if min_time > elapsed_time:
        min_time = elapsed_time
        min_word = word
print(min_time, min_word)
print(max_time, max_word)

start = perf_counter()
for i in range(1000):
    segmentation('НЕТЕГО', gl_seg)
elapsed_time = perf_counter() - start
print(elapsed_time)
