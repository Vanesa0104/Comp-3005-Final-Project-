-- DEFAULT DATA

-- Default Members
INSERT INTO Member (name, email, date_of_birth, gender, contact) VALUES
('Ash Ketchum', 'ash.ketchum@example.com', '2001-05-22', 'Male', '123-456-7890'),
('Misty Waterflower', 'misty.water@example.com', '2001-07-15', 'Female', '098-765-4321'),
('Brock Stone', 'brock.stone@example.com', '2000-11-05', 'Male', '147-258-3690');

-- Default Trainers
INSERT INTO Trainer (name, email, specialty) VALUES
('Cynthia Sinnoh', 'cynthia.sinnoh@example.com', 'Pilates'),
('Tracey Sketchit', 'tracey.sketchit@example.com', 'Cardio'),
('Lance Kanto', 'lance.kanto@example.com', 'Strength Training');

-- Default Admins
INSERT INTO Admin (name, email) VALUES
('Vanesa Smith', 'vanesa.smith@example.com'),
('John Doe', 'john.doe@example.com'),
('Augustine Sycamore', 'augustine.sycamore@example.com');

-- Default Rooms
INSERT INTO Room (room_name) VALUES
('Room A'), ('Room B'), ('Room C'), ('Room D'),
('Room E'), ('Room F'), ('Room G'), ('Room H');

-- Default PT Sessions
INSERT INTO PTSession (member_id, trainer_id, start_time, end_time, room) VALUES
(1, 1, '2025-11-25 15:00:00', '2025-11-25 16:00:00', 'Room A'),
(2, 2, '2025-11-25 09:00:00', '2025-11-25 10:00:00', 'Room B'),
(3, 3, '2025-11-26 11:00:00', '2025-11-26 12:00:00', 'Room C');

-- Default Trainer Availability
INSERT INTO TrainerAvailability (trainer_id, start_time, end_time) VALUES
(1, '2025-11-25 15:00:00', '2025-11-25 17:00:00'),
(1, '2025-11-25 11:00:00', '2025-11-25 13:30:00'),
(1, '2025-11-25 14:00:00', '2025-11-25 15:00:00'),
(2, '2025-11-25 09:00:00', '2025-11-25 12:00:00');

-- Default Health Metrics
INSERT INTO HealthMetric (member_id, metric_time, weight, height, heart_rate) VALUES
(1, '2025-11-24 08:00:00', 70.5, 175, 72),
(1, '2025-11-25 08:00:00', 70.0, 175, 70),
(2, '2025-11-25 09:00:00', 85.0, 180, 75),
(3, '2025-11-25 10:00:00', 60.0, 165, 68);

-- Default Group Classes
INSERT INTO GroupClass (class_name, trainer_id, class_time, capacity) VALUES
('Yoga Morning', 2, '2025-11-25 08:00:00', 15),
('HIIT Training', 1, '2025-11-25 10:00:00', 10),
('Cardio Blast', 3, '2025-11-26 09:00:00', 20);

-- Default Member Class Attendance
INSERT INTO MemberClass (member_id, class_id) VALUES
(1, 1),
(2, 2),
(3, 3);

-- Default Fitness Goals
INSERT INTO FitnessGoal (member_id, start_date, end_date, target_weight, target_body_fat, active) VALUES
(1, '2025-11-01', '2025-12-01', 65.0, 18.0, TRUE),
(2, '2025-11-05', '2025-12-05', 80.0, 20.0, TRUE),
(3, '2025-11-10', '2025-12-10', 58.0, 22.0, TRUE);
 
-- Default Equipment
INSERT INTO Equipment (equip_name) VALUES
('Dumbbells'),
('Treadmill'),
('Yoga Mats');
