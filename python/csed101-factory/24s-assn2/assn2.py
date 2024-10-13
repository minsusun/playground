import os

student_list = dict()

def show():
    print("Student       Name        ")

score_file_name = input("Input the score file name: ").strip()

if not os.path.exists(score_file_name):
    print(f"The file '{score_file_name}' does not exist.")
    exit()

with open(score_file_name, "r") as f:
    for line in f:
        student_id, name, midterm, final = line.split()
        midterm = int(midterm)
        final = int(final)
        average = (midterm + final) / 2
        grade = str()

        if average >= 90:
            grade = 'A'
        elif average >= 80:
            grade = 'B'
        elif average >= 70:
            grade = 'C'
        elif average >= 60:
            grade = 'D'
        else:
            grade = 'E'

        student_list[student_id] = {
            'name': name,
            'midterm': midterm,
            'final': final,
            'average': average,
            'grade': grade
        }

student_list = sorted(student_list, key = lambda x: x['average'])

while True:
    pass

