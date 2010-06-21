
char_1 = {};
function char_1.do_spawn(self)
	
	-- 捆绑武器
	to_bind_weapon(self, 10070001, "R");

	-- 注册UI
	ui_reg_char_info(self);
	
	-- 设置基本技能
	-- to_learn_skill(self, 4);
	
end

function char_1.do_walk(self)
	
	set_next_anim(self,"run",-1);
	
end

function char_1.do_idle(self)
	
	set_next_anim(self,"idle",-1);
	
end

function char_1.do_death(self)


end

function char_1.do_hit(self)

end