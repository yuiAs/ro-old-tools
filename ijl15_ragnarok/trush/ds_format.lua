
-- os.date
-- http://www.lua.org/pil/22.1.html



-- normal

function chat_normal(arg1, body)
	return string.format("NOR %s %s\n", os.date("%H:%M:%S"), body)
end

-- party

function chat_party(arg1, body)
	return string.format("PRT %s %s\n", os.date("%H:%M:%S"), body)
end

-- guild

function chat_guild(arg1, body)
	return string.format("GLD %s %s\n", os.date("%H:%M:%S"), body)
end

-- whisper_send

function chat_wis_s(name, body)
	return string.format("WIS %s TO %s : %s\n", os.date("%H:%M:%S"), name, body)
end

-- whisper_recv

function chat_wis_r(name, body)
	return string.format("WIS %s FROM %s : %s\n", os.date("%H:%M:%S"), name, body)
end

-- broadcast

function chat_broadcast(arg1, body)
	return string.format("GOD %s %s\n", os.date("%H:%M:%S"), body)
end

-- localbroadcast

function chat_lbc(arg1, body)
	return string.format("LBC %s %s\n", os.date("%H:%M:%S"), body)
end

-- talkie

function chat_talkie(arg1, body)
	return string.format("TLK %s %s\n", os.date("%H:%M:%S"), body)
end



-- systemlog

function sys_char(name)
	return string.format("SYS %s has connected\n", name)
end

function sys_zone(map)
	return string.format("MAP %s (%s)\n", map, os.date("%Y-%m-%d %H:%M:%S"))
end

function sys_mvp(aid)
--	return string.format("MVP %s AID=%08X\n", os.date("%H:%M:%S"), aid)
end
