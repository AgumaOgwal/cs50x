
def main():
    a = "Hello"
    b= "Temperature"
    
    final = substrings(b,2) 
    
    for word in final:
        print(word)

def substrings(a,n):
    list = []
    for i in range(len(a)):
        substr = a[i:n]
        list.append(substr)
        n = n + 1
        if n > len(a):
            break
    return list
            
     
if __name__ == "__main__":
    main()