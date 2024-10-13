import os

global student_list = dict()

score_file_name=input("Input the score file name: ").strip()

if not os.path.exists(score_file_name):
    print(f"The file '{score_file_name}' does not exist.")
    exit()
    
with open(score_file_name, "r") as f:
    for line in f:
        student_id, name, midterm, final = line.split()
        midterm = int(midterm)
        final = int(final)
        avg = (midterm + final) / 2
        grade = ''
        
        if avg >= 90:
            grade = 'A'
        elif avg >= 80:
            grade ='B'
        elif avg >= 70:
            grade ='C'
        elif avg >= 60:
            grade ='D'
        else:
            grade ='E'
            
        student_list[student_id] = {
            'name':name, 
            'midterm':midterm, 
            'final':final, 
            'avg':avg, 
            'grade': grade
        }
        
        
start=input()

if start == "show":
    show()
elif start == "search":
    search()
elif start == "changescore":
    changescore()
elif start == "add":
    add()
elif start == "getstats":
    getstats()
elif start == "remove":
    remove()
elif start == "quit":
    quit()
    
        
def show():
    if not student_list:
        print("LIST IS EMPTY.")
    else: 
        sorted_sl = sorted(student_list.items(), key = lambda a: a[1]['avg'])
        for list in student_list:
        
            print("Student\tName\tMidterm\tFinal\tAverage\tGrade")
            print("-----------------------------------------------------------------------")
        
            print("%19s" %(sorted_sl[list]), end='')
            print("%19s" %(sorted_sl[list]['name']), end='')
            print("%19d" %(sorted_sl[list]['midterm']), end='')
            print("%19d" %(sorted_sl[list]['final']), end='')
            print("%19.1f" %(sorted_sl[list]['avg']), end='')
            print(sorted_sl[list]['grade'], end='')
        
        
        
def search():
    if not student_list:
        print("LIST IS EMPTY.")
    else:  
        id= int(input("Student ID:"))
        if id in student_list:
            print("Student\tName\tMidterm\tFinal\tAverage\tGrade")
            print("-----------------------------------------------------------------------")
        
        
            print("%19d" %id, end='')
            print("%19s" %(student_list[id]['name']), end='')
            print("%19d" %(student_list[id]['midterm']), end='')
            print("%19d" %(student_list[id]['final']), end='')
            print("%19.1f" %(student_list[id]['avg']), end='')
            print(student_list[0]['grade'], end='')
            
        else:
            print("NO SUCH PERSON.")
            
            
def changescore():
    id= int(input("Student ID:"))
    
    if not student_list:
        print("LIST IS EMPTY.")
    else:
        if id in student_list:
            mf=input("Mid/Final:").lower()
            new=int(input("Input new score:"))
            
            if not 0<=new && new<=100:
                print("Student\tName\tMidterm\tFinal\tAverage\tGrade")
                print("-----------------------------------------------------------------------")
        
                print("%19s" %(student_list[id]), end='')
                print("%19s" %(student_list[id]['name']), end='')
                print("%19d" %(student_list[id]['midterm']), end='')
                print("%19d" %(student_list[id]['final']), end='')
                print("%19.1f" %(student_list[id]['avg']), end='')
                print(student_list[0]['grade'])
                      
                print("Score changed.")
                      
                if mf == 'mid':
                    student_list[id]['midterm']=new
                    student_list[id]['grade']=(new+student_list[id]['final'])/2
                else: student_list[id]['final']=new 
                      student_list[id]['grade']=(new+student_list[id]['mideterm'])/2
            else: print("INVALID SCORE.")
        else: print("NO SUCH PERSON.")
                      
            print("%19s" %(student_list[id]), end='')
            print("%19s" %(student_list[id]['name']), end='')
            print("%19d" %(student_list[id]['midterm']), end='')
            print("%19d" %(student_list[id]['final']), end='')
            print("%19.1f" %(student_list[id]['avg']), end='')
            print(student_list[0]['grade'])
                      
def add():
    id= int(input("Student ID:"))
    if not id in student_list:
        name = input("Name:")
        mid = int(input("Midterm Score:"))
        fin = int(input("Final Score:"))
        print("Student added.")            
        avg = (mid + fin) / 2
                      
        student_list[id]= {
            'name'=name,
            'midterm' = mid,
            'final' = fin,
            'avg' = avg,
            'grade' = ''
            
            if avg >= 90:
                grade = 'A'
            elif avg >= 80:
                grade ='B'
            elif avg >= 70:
                grade ='C'
            elif avg >= 60:
                grade ='D'
            else:
                grade ='E'
        }              
        
    else: print("ALREADY EXISTS.")
                      
    
def searchgrade():
    grade = input("Grade to search:")
    if not student_list:
        print("LIST IS EMPTY.")
    else:                 
        if not ord('A') <= ord(grade) && ord(grade) <= ord('E'):
            for list in student_list:
                if grade == student_list[list]['grade']:
                    print("Student\tName\tMidterm\tFinal\tAverage\tGrade")
                    print("-----------------------------------------------------------------------")
         
                    print("%19s" %(student_list[id]), end='')
                    print("%19s" %(student_list[id]['name']), end='')
                    print("%19d" %(student_list[id]['midterm']), end='')
                    print("%19d" %(student_list[id]['final']), end='')
                    print("%19.1f" %(student_list[id]['avg']), end='')
                    print(student_list[0]['grade'])
                      
        else: print("INVALID GRADE.")


def getstats():
    if not student_list:
        print("LIST IS EMPTY.")
    else:   
        mid=[]  
        fin=[]
        avg=[]
        c=0              
        for list in student_list():
            mid.append(student_list[list]['midterm'])
            fin.append(student_list[list]['final'])
            avg.append(student_list[list][avg])
            c++
              
    
                      
        sorted_mid = sorted(mid) 
        sorted_fin = sorted(fin) 
        sorted_avg = sorted(avg)               
        if c%2==0:
            med_m=(sorted_mid[c//2] + sorted_mid[c//2+1])/2
            med_f=(sorted_fin[c//2] + sorted_fin[c//2+1])/2
            med_a=(sorted_avg[c//2] + sorted_avg[c//2+1])/2
        else: 
            med_m = sorted_mid[c/2]
            med_f = sorted_fin[c/2]
            med_a = sorted_avg[c/2]
   
                      
        avg_m = sum(mid) / c
        avg_f = sum(fin) / c
        avg_a = sum(avg) / c 
                      
    
        for i in range(c):
            std_m += (mid[i]-avg_m)**2
            std_f += (mid[i]-avg_m)**2
            std_m += (mid[i]-avg_m)**2
                      
        std_m /=c                  
        std_f /=c 
        std_a /=c 
    
        std_m *=0.5                 
        std_f *=0.5 
        std_a *=0.5
                    
    print("State\tMidterm\tFinal\tAverage")
    print("-----------------------------------------------------------------------")
         
    print("%19s" %(med_m, end='')
    print("%19s" %(med_f, end='')
    print("%19s" %(med_a))
      
    print("%19s" %(avg_m, end='')
    print("%19s" %(avg_f, end='')
    print("%19s" %(avg_a))
      
    print("%19s" %(std_m, end='')
    print("%19s" %(std_f, end='')
    print("%19s" %(std_a))
          

def remove():
    if not student_list:
        print("LIST IS EMPTY.")
    else:      
        id = int(input("Student ID:"))
        ans = input(f"Remove {id}? [yes/no]") # "Remove {}? [yes/no]".format(id)
        
        for list in student_list:
            if not ans in list:
                print("NO SUCH PERSON.")
            else: 
                if ans == 'no':
                    print("Cancelled.")
                    break
                else: 
                    print("Student removed.")
                    break
          
def quit():
    save = input("Save data? [yes/no]")
    if save == 'yes':
        file = input("File name:")
    else: 
        with open(score_file_name, "w") as f:
            sorted_sl2 = sorted(student_list.items(), key = lambda a: a[1]['avg'])
        for list in student_list:
        
            print("Student\tName\tMidterm\tFinal\n")
            print("-----------------------------------------------------------------------")
        
            print("%19s" %(sorted_sl2[list]), end='')
            print("%19s" %(sorted_sl2[list]['name']), end='')
            print("%19d" %(sorted_sl2[list]['midterm']), end='')
            print("%19d" %(sorted_sl2[list]['final']), end='')
