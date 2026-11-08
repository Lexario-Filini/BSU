USE [Расписание занятий];
GO

CREATE TABLE Корпусы (
Код_корпуса INT PRIMARY KEY,
Название NVARCHAR(100) NOT NULL
);

CREATE TABLE Тип_аудиторий (
Код_типа_аудиторий INT PRIMARY KEY,
Название NVARCHAR(100) NOT NULL
);

CREATE TABLE Преподаватели (
Код_преподавателя INT PRIMARY KEY,
ФИО NVARCHAR(150) NOT NULL
);

CREATE TABLE Предметы (
Код_предмета INT PRIMARY KEY,
Название NVARCHAR(150) NOT NULL
);

CREATE TABLE Аудитории (
Код_аудитории INT PRIMARY KEY,
Код_корпуса INT FOREIGN KEY REFERENCES Корпусы(Код_корпуса),
Код_типа INT FOREIGN KEY REFERENCES Тип_аудиторий(Код_типа_аудиторий),
Номер NVARCHAR(10) NOT NULL,
Количество_мест INT
);

CREATE TABLE Учебный_план (
Код_плана INT PRIMARY KEY,
Код_преподавателя INT FOREIGN KEY REFERENCES Преподаватели(Код_преподавателя),
Код_предмета INT FOREIGN KEY REFERENCES Предметы(Код_предмета),
Количество_пар_в_неделю INT
);

CREATE TABLE Расписание (
Код_записи INT PRIMARY KEY,
Код_преподавателя INT FOREIGN KEY REFERENCES Преподаватели(Код_преподавателя),
Код_предмета INT FOREIGN KEY REFERENCES Предметы(Код_предмета),
Код_аудитории INT FOREIGN KEY REFERENCES Аудитории(Код_аудитории),
Код_учебного_плана INT FOREIGN KEY REFERENCES Учебный_план(Код_плана),
День_недели NVARCHAR(20),
Номер_пары INT,
Количество_студентов INT,
);

CREATE TABLE Требуемый_тип_аудиторий (
Код_требуемого_типа INT PRIMARY KEY,
Код_предмета INT FOREIGN KEY REFERENCES Предметы(Код_предмета),
Код_типа_аудитории INT FOREIGN KEY REFERENCES Тип_аудиторий(Код_типа_аудиторий)
)

