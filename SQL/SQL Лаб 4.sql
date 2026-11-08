CREATE TABLE Работники (
Код_работника INT PRIMARY KEY IDENTITY(1,1),
Фамилия NVARCHAR(50) NOT NULL,
Имя NVARCHAR(50) NOT NULL,
Должность NVARCHAR(50) DEFAULT 'Рабочий',
Телефон NVARCHAR(20) CONSTRAINT CHK_Phone_Format CHECK (Телефон LIKE '+375%'),
Дата_приема DATE NOT NULL,
);

CREATE TABLE Продукция (
Код_продукции INT PRIMARY KEY IDENTITY(1,1),
Название NVARCHAR(50) NOT NULL,
Тип NVARCHAR(50) NOT NULL,
Себестоимость DECIMAL(18,2) CONSTRAINT CHK_Cost CHECK (Себестоимость > 0),
Цена DECIMAL(18,2) CONSTRAINT CHK_Price CHECK (Цена > 0),
CONSTRAINT CHK_Price_Higher CHECK (Цена >= Себестоимость),
);

CREATE TABLE Производство (
Код_операции INT PRIMARY KEY IDENTITY(1,1),
Код_работника INT NOT NULL,
Код_продукции INT NOT NULL,
Дата_производства DATE DEFAULT GETDATE(),
Количество INT CONSTRAINT CHK_Quantity CHECK (Количество > 0),

CONSTRAINT FK_Worker FOREIGN KEY (Код_работника)
REFERENCES Работники (Код_работника)
ON DELETE CASCADE ON UPDATE CASCADE,

CONSTRAINT FK_Product FOREIGN KEY (Код_продукции)
REFERENCES Продукция (Код_продукции)
ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE INDEX IX_Workers_LastName  ON Работники(Фамилия);
CREATE INDEX IX_Products_Type ON Продукция(Тип);
CREATE INDEX UX_Production_Date_Qty On Производство(Дата_производства, Количество);

INSERT INTO Работники (Фамилия, Имя, Должность, Телефон, Дата_приема)
VALUES 
(N'Иванов', N'Иван', N'Мастер', '+375293555050', '2023-01-15'),
(N'Петров', N'Петр', N'Сборщик', '+375291234567', '2023-05-20');

INSERT INTO Продукция (Название, Тип, Себестоимость, Цена)
VALUES 
(N'Стол кухонный', N'Мебель', 1500.00, 3000.00),
(N'Стул офисный', N'Мебель', 800.00, 1500.00);

INSERT INTO Производство (Код_работника, Код_продукции, Количество)
VALUES 
(1, 1, 10),
(2, 2, 50);