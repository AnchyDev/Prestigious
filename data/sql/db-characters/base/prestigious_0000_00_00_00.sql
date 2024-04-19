CREATE TABLE IF NOT EXISTS `prestige_tracker` (
  `guid` int NOT NULL DEFAULT '0',
  `prestigeLevel` int DEFAULT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;