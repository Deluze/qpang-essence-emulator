CREATE TABLE `player_skill_cards` (
  `player_id` bigint unsigned NOT NULL,
  `skill_1_card_id` bigint unsigned NOT NULL,
  `skill_2_card_id` bigint unsigned NOT NULL,
  `skill_3_card_id` bigint unsigned NOT NULL,
  PRIMARY KEY (`player_id`),
  CONSTRAINT `player_skills_player_id_fk_id` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
SELECT * FROM qfighter.player_skill_cards;