CREATE TABLE IF NOT EXISTS `prestige_sacrificed` (
  `player` int NOT NULL,
  `itemlevel` int NOT NULL,
  PRIMARY KEY (`player`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;