# first action item
# Use sent_tokenize from the Natural Language Toolkit to "tokenize" (i.e., separate) each string into a list of sentences
from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    # First, split strings
    # a bug with check 50. had to include "\n" inside split
    # because it was splitting using spaces. hence lines with spaces would be broken into multiple lines
    c = a.split("\n")
    d = b.split("\n")
    list = []
    for i in c:
        for j in d:
            if i == j:
                # count() checks how many times a given item appears in a list
                if list.count(i) < 1:
                    list.append(i)
                break
    # TODO
    return list


def sentences(a, b):
    """Return sentences in both a and b"""
    c = sent_tokenize(a)
    d = sent_tokenize(b)
    sentences = []
    for i in c:
        for j in d:
            if i == j:
                # count() checks how many times a given item appears in a list
                if sentences.count(i) < 1:
                    sentences.append(i)
                break
    # TODO
    return sentences


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    # TODO
    substringsA = getsubstrings(a, n)
    substringsB = getsubstrings(b, n)
    # an empty list to hold our common substrings
    substrings = []
    # time to check identify all substrings that are held in common between a and b
    for i in substringsA:
        for j in substringsB:
            if i == j:
                # count() checks how many times a given item appears in a list
                if substrings.count(i) < 1:
                    substrings.append(i)
                break
    return substrings

# the below is a helper method to extract subtrings
# accepting just one string at a time


def getsubstrings(a, n):
    # house keeping check first. to not return anysubstring longer than string length
    if n > len(a):
        return []
    # first we need a list which will finally contain our substrings
    list = []
    # iterate over the supplied string starting from index 0. first substring will start from index 0
    for i in range(len(a)):
        # built in string method s[x:y] will extract substring from index x upto and excluding y
        substr = a[i:n]
        # add the resulting substring to our list
        list.append(substr)
        # move the y part of our substring by 1 to the right. so it moves with i uniformly, hence maintaining the desired substring length
        n = n + 1
        # once n has exceeded length of the string, such that any further substrings wont be of desired length n;
        # its time to break out of the loop, and return our list shortly afterwards
        if n > len(a):
            break
    return list