CREATE TABLE `redeem_codes` (
  `id` int NOT NULL AUTO_INCREMENT,
  `player_id` bigint unsigned NOT NULL DEFAULT '0',
  `redeem_code` char(14) NOT NULL,
  `total_uses` int unsigned NOT NULL DEFAULT '0',
  `use_count` int unsigned NOT NULL DEFAULT '0',
  `cash` int unsigned NOT NULL DEFAULT '0',
  `don` int unsigned NOT NULL DEFAULT '0',
  `active` tinyint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `redeem_code_UNIQUE` (`redeem_code`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='In this table, all of the redeem codes get stored.';

CREATE TABLE `redeem_cards` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `item_id` int unsigned NOT NULL,
  `type` tinyint unsigned NOT NULL,
  `period` int unsigned NOT NULL,
  `period_type` tinyint unsigned NOT NULL,
  `is_giftable` tinyint unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='In this table, all cards that can be used for redeem codes get stored.';

CREATE TABLE `redeem_code_redeem_cards` (
  `redeem_code_id` int NOT NULL,
  `redeem_card_id` int NOT NULL,
  PRIMARY KEY (`redeem_card_id`,`redeem_code_id`),
  KEY `redeem_code_redeem_cards_redeem_code_fk_id_idx` (`redeem_code_id`),
  CONSTRAINT `redeem_code_redeem_cards_redeem_card_fk_id` FOREIGN KEY (`redeem_card_id`) REFERENCES `redeem_cards` (`id`),
  CONSTRAINT `redeem_code_redeem_cards_redeem_code_fk_id` FOREIGN KEY (`redeem_code_id`) REFERENCES `redeem_codes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='In this table, the relation between a redeem code and a card gets stored.';

CREATE TABLE `redeem_code_uses` (
  `redeem_code_id` int NOT NULL,
  `player_id` bigint unsigned NOT NULL,
  `use_count` int unsigned NOT NULL,
  PRIMARY KEY (`redeem_code_id`,`player_id`),
  KEY `redeem_code_id_fk_idx` (`redeem_code_id`) /*!80000 INVISIBLE */,
  KEY `redeem_code_uses_player_fk_id_idx` (`player_id`),
  CONSTRAINT `redeem_code_uses_player_fk_id` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`),
  CONSTRAINT `redeem_code_uses_redeem_code_fk_id` FOREIGN KEY (`redeem_code_id`) REFERENCES `redeem_codes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='In this table, the amount of times a certain player has used a redeem code gets stored.';