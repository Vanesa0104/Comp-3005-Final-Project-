-- Connect to default postgres database
\c postgres;

-- Drop & create fitness database
DROP DATABASE IF EXISTS fitness;
CREATE DATABASE fitness;

-- Connect to fitness database
\c fitness;

-- Members table
CREATE TABLE Member (
    member_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    date_of_birth DATE NOT NULL,
    gender VARCHAR(10) NOT NULL,
    contact VARCHAR(50) NOT NULL
);

-- Trainers table
CREATE TABLE Trainer (
    trainer_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    specialty VARCHAR(50) NOT NULL
);

-- Admins table
CREATE TABLE Admin (
    admin_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL
);

-- Personal Training Sessions
CREATE TABLE PTSession (
    session_id SERIAL PRIMARY KEY,
    member_id INT REFERENCES Member(member_id),
    trainer_id INT REFERENCES Trainer(trainer_id),
    start_time TIMESTAMP NOT NULL,
    end_time TIMESTAMP NOT NULL,
    room VARCHAR(50) NOT NULL
);

-- Group Classes
CREATE TABLE GroupClass (
    class_id SERIAL PRIMARY KEY,
    class_name VARCHAR(100) NOT NULL,
    trainer_id INT REFERENCES Trainer(trainer_id),
    class_time TIMESTAMP NOT NULL,
    capacity INT NOT NULL
);

-- Health Metrics (height added)
CREATE TABLE HealthMetric (
    metric_id SERIAL PRIMARY KEY,
    member_id INT REFERENCES Member(member_id),
    weight DECIMAL(5,2) NOT NULL,
    height DECIMAL(5,2) NOT NULL,
    heart_rate INT NOT NULL,
    metric_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Fitness Goals
CREATE TABLE FitnessGoal (
    goal_id SERIAL PRIMARY KEY,
    member_id INT REFERENCES Member(member_id),
    target_weight DECIMAL(5,2) NOT NULL,
    target_body_fat DECIMAL(5,2),
    start_date DATE NOT NULL,
    end_date DATE NOT NULL,
    active BOOLEAN DEFAULT TRUE
);

-- Trainer availability table
CREATE TABLE TrainerAvailability (
    availability_id SERIAL PRIMARY KEY,
    trainer_id INT REFERENCES Trainer(trainer_id),
    start_time TIMESTAMP NOT NULL,
    end_time TIMESTAMP NOT NULL
);

-- Equipment
CREATE TABLE Equipment (
    equipment_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);

-- Maintenance log
CREATE TABLE MaintenanceLog (
    maintenance_id SERIAL PRIMARY KEY,
    equipment_id INT REFERENCES Equipment(equipment_id),
    issue_description TEXT NOT NULL,
    assigned_to INT REFERENCES Admin(admin_id),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

--- Default Data

-- Default Admins
INSERT INTO Admin (name, email) VALUES
('Vanesa Smith', 'vanesa.smith@example.com'),
('John Doe', 'john.doe@example.com')
ON CONFLICT (email) DO NOTHING;

-- Default Members
INSERT INTO Member (name, email, date_of_birth, gender, contact) VALUES
('Ash Ketchum', 'ash.ketchum@example.com', '2000-05-22', 'Male', '123-456-7890'),
('Misty Waterflower', 'misty.water@example.com', '2001-07-15', 'Female', '098-765-4321')
ON CONFLICT (email) DO NOTHING;

-- Default Trainers
INSERT INTO Trainer (name, email, specialty) VALUES
('Brock Stone', 'brock.stone@example.com', 'Strength Training'),
('Tracey Sketchit', 'tracey.sketchit@example.com', 'Cardio')
ON CONFLICT (email) DO NOTHING;

-- Default Equipment
INSERT INTO Equipment (name) VALUES
('Dumbbells'),
('Treadmill'),
('Yoga Mats')
ON CONFLICT (name) DO NOTHING;
