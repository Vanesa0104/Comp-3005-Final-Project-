-- CONNECT & DATABASE SETUP

\c postgres;

-- DROP DATABASE IF EXISTS fitness;
CREATE DATABASE fitness;

\c fitness;

-- TABLES

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

-- Rooms table
CREATE TABLE Room (
    room_id SERIAL PRIMARY KEY,
    room_name VARCHAR(50) UNIQUE NOT NULL
);

-- Personal Training Sessions
CREATE TABLE PTSession (
    session_id SERIAL PRIMARY KEY,
    member_id INT REFERENCES Member(member_id),
    trainer_id INT REFERENCES Trainer(trainer_id),
    start_time TIMESTAMP NOT NULL,
    end_time TIMESTAMP NOT NULL,
    room VARCHAR(50) NOT NULL,
    FOREIGN KEY (room) REFERENCES Room(room_name)
);

-- Group Classes
CREATE TABLE GroupClass (
    class_id SERIAL PRIMARY KEY,
    class_name VARCHAR(100) NOT NULL,
    trainer_id INT REFERENCES Trainer(trainer_id),
    class_time TIMESTAMP NOT NULL,
    capacity INT NOT NULL
);

-- Health Metrics
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

-- Trainer availability
CREATE TABLE TrainerAvailability (
    availability_id SERIAL PRIMARY KEY,
    trainer_id INT REFERENCES Trainer(trainer_id),
    start_time TIMESTAMP NOT NULL,
    end_time TIMESTAMP NOT NULL
);

-- Equipment
CREATE TABLE Equipment (
    equipment_id SERIAL PRIMARY KEY,
    equip_name VARCHAR(100) UNIQUE NOT NULL
);

-- Maintenance log
CREATE TABLE MaintenanceLog (
    maintenance_id SERIAL PRIMARY KEY,
    equipment_id INT REFERENCES Equipment(equipment_id),
    issue_description TEXT NOT NULL,
    assigned_to VARCHAR(100) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status VARCHAR(50) DEFAULT 'Pending',
    notes TEXT
);

CREATE TABLE MemberClass (
    member_id INT REFERENCES Member(member_id),
    class_id INT REFERENCES GroupClass(class_id),
    PRIMARY KEY (member_id, class_id)
);

-- VIEW

CREATE OR REPLACE VIEW MemberDashboardView AS
SELECT 
    m.member_id,
    m.name,
    m.email,

    (SELECT weight FROM HealthMetric hm 
     WHERE hm.member_id = m.member_id 
     ORDER BY metric_time DESC LIMIT 1) AS latest_weight,

    (SELECT height FROM HealthMetric hm 
     WHERE hm.member_id = m.member_id 
     ORDER BY metric_time DESC LIMIT 1) AS latest_height,

    (SELECT heart_rate FROM HealthMetric hm 
     WHERE hm.member_id = m.member_id 
     ORDER BY metric_time DESC LIMIT 1) AS latest_heart_rate,

    (SELECT target_weight FROM FitnessGoal fg 
     WHERE fg.member_id = m.member_id AND fg.active = TRUE 
     ORDER BY start_date DESC LIMIT 1) AS active_target_weight,

    (SELECT target_body_fat FROM FitnessGoal fg 
     WHERE fg.member_id = m.member_id AND fg.active = TRUE 
     ORDER BY start_date DESC LIMIT 1) AS active_target_body_fat

FROM Member m;

-- TRIGGER FUNCTION

CREATE OR REPLACE FUNCTION prevent_trainer_overlap()
RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1 FROM TrainerAvailability ta
        WHERE ta.trainer_id = NEW.trainer_id
        AND (NEW.start_time, NEW.end_time) OVERLAPS (ta.start_time, ta.end_time)
    ) THEN
        RAISE EXCEPTION 'Trainer availability overlaps with an existing time slot.';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- TRIGGER

CREATE TRIGGER check_trainer_availability_overlap
BEFORE INSERT OR UPDATE ON TrainerAvailability
FOR EACH ROW
EXECUTE FUNCTION prevent_trainer_overlap();

-- INDEXES

-- Speed up trainer time conflict checks
CREATE INDEX idx_ptsession_trainer_time
ON PTSession (trainer_id, start_time, end_time);

-- Speed up name lookups for trainer screen
CREATE INDEX idx_member_name ON Member(name);

-- Speed up room schedule lookups
CREATE INDEX idx_room_name ON Room(room_name);
