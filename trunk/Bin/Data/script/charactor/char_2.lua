
char_2 = {};
function char_2.do_spawn(self)
	
	--添加路径点
	--add_pathway_pos(self,0,0);
	--add_pathway_pos(self,10,20);
	
end

function char_2.do_walk(self)
	
	set_next_anim(self,"Run",-1);
	
end

function char_2.do_idle(self)
	
	set_next_anim(self,"idle",-1);
	
end

function char_2.do_death(self)
	
	set_next_anim(self,"Die",-1);
	
end

function char_2.do_hit(self)

end