USE qfighter;

# 0 = MELEE
UPDATE weapons SET type = 0 WHERE
	item_id = 1095172098 OR
	item_id = 1095172100 OR
	item_id = 1095172102 OR
	item_id = 1095172103 OR
	item_id = 1095172104 OR
	item_id = 1095172105 OR
	item_id = 1095172130 OR
	item_id = 1095172131 OR
	item_id = 1095761958 OR
	item_id = 1095172147 OR
	item_id = 1095172160 OR
	item_id = 1095172097;

# 1 = RIFLE
UPDATE weapons SET type = 1 WHERE
	item_id = 1095237632 OR
	item_id = 1095303169 OR
	item_id = 1095761921 OR
	item_id = 1095303170 OR
	item_id = 1095303171 OR
	item_id = 1095303172 OR
	item_id = 1095303173 OR
	item_id = 1095303177 OR
	item_id = 1095303179 OR
	item_id = 1095303180 OR
	item_id = 1095303181 OR
	item_id = 1095368720 OR
	item_id = 1095303185 OR
	item_id = 1095237671 OR
	item_id = 1095237672 OR
	item_id = 1095761961 OR
	item_id = 1095303231 OR
	item_id = 1095303233;
    
# 2 = LAUNCHER
UPDATE weapons SET type = 2 WHERE
	item_id = 1095368704 OR
	item_id = 1095368706 OR
	item_id = 1095368707 OR
	item_id = 1095368708 OR
	item_id = 1095368709 OR
	item_id = 1095368710 OR
	item_id = 1095368711 OR
	item_id = 1095368713 OR
	item_id = 1095368716 OR
	item_id = 1095368717 OR
	item_id = 1095434253 OR
	item_id = 1095368718 OR
	item_id = 1095368719 OR
	item_id = 1095368721 OR
	item_id = 1095368734 OR
	item_id = 1095368746 OR
	item_id = 1095368756 OR
	item_id = 1095368757 OR
	item_id = 1095368758 OR
	item_id = 1095368764 OR
	item_id = 1095368766;
    
# 3 = BOMB
UPDATE weapons SET type = 3 WHERE
	item_id = 1095434240 OR
	item_id = 1095499776 OR
	item_id = 1095434241 OR
	item_id = 1095499777 OR
	item_id = 1095434242 OR
	item_id = 1095499778 OR
	item_id = 1095434243 OR
	item_id = 1095499779 OR
	item_id = 1095434244 OR
	item_id = 1095434246 OR
	item_id = 1095434249 OR
	item_id = 1095434252 OR
	item_id = 1095434255 OR
	item_id = 1095434256 OR
	item_id = 1095434257 OR
	item_id = 1095434258 OR
	item_id = 1095434277 OR
	item_id = 1095434285 OR
	item_id = 1095434287 OR
	item_id = 1095434290 OR
	item_id = 1095434295 OR
	item_id = 1095434296 OR
	item_id = 1095434297 OR
	item_id = 1095434299 OR
	item_id = 1095499837;

# Vul het type voor de rest van de wapens in met -1, want deze zijn nog niet bekend.
UPDATE weapons SET type = 999 WHERE type IS NULL;

SELECT * FROM weapons
ORDER BY type;