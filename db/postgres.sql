CREATE TABLE Demo(
    "id"      SERIAL PRIMARY KEY, 
    "name"    TEXT,
    "age"     INT,
    "married" BOOLEAN
);

INSERT INTO Demo(name, age, married) VALUES
    ('user-1', 42, TRUE), 
    ('user-2', 22, FALSE), 
    ('user-3', 55, TRUE);