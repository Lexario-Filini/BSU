USE [Учет издержек];
GO

CREATE TABLE Статьи_расхода (
    Код_статьи INT PRIMARY KEY, 
    Наименование VARCHAR(255) NOT NULL 
);

CREATE TABLE Подразделения (
    Код_подразделения INT PRIMARY KEY, 
    Название VARCHAR(255) NOT NULL 
);

CREATE TABLE Ведомость_издержек (
    Код_записи INT PRIMARY KEY IDENTITY(1,1),
    Номер_ведомости VARCHAR(50), 
    Дата_ведомости DATE, 
    Код_подразделения INT,
    Код_статьи INT, 
    Сумма DECIMAL(15, 2), 
    Примечание TEXT, 
    FOREIGN KEY (Код_подразделения) REFERENCES Подразделения(Код_подразделения),
    FOREIGN KEY (Код_статьи) REFERENCES Статьи_расхода(Код_статьи)
);

CREATE TABLE Товарооборот_подразделений (
    Идентификатор_записи INT PRIMARY KEY IDENTITY(1,1),
    Месяц_отчета DATE, 
    Код_подразделения INT, 
    Сумма_товарооборота DECIMAL(15, 2), 
    FOREIGN KEY (Код_подразделения) REFERENCES Подразделения(Код_подразделения)
);

