
drop table if exists cdr_data;
create table cdr_data
(
	id       		int auto_increment not null primary key, 
	site_number     varchar(20) not null,
	report_time   	datetime not null, 
	deepness 		float(8, 3) not null,
	temperature 	float(8, 3) not null,
	battery_level 	int not null
)default charset=utf8;

drop table if exists cdr_site_info;
create table cdr_site_info
(
	hole_id             int auto_increment not null primary key, 
	hole_number         varchar(20) not null,
	hole_coordinate_X   varchar(20) not null,
	hole_coordinate_Y   varchar(20) not null,
	hole_coordinate_Z   varchar(20) not null,
	hole_elevation      float(8, 3) not null,
	hole_depth          float(8, 3) not null,
	geological          varchar(20) not null,
	water_elevation     float(8, 3) not null,
	
	site_number   	varchar(20) not null,
	site_phone  	varchar(20) not null,
	site_status  	varchar(20) not null,
	line_lenght  	float(8, 3) not null,
	calibration     float(8, 3) not null,
	location            varchar(100)
) default charset=utf8; 


update cdr_site_info set hole_coordinate_X='111', hole_coordinate_Y='222', hole_coordinate_Z='333', hole_elevation=444 , hole_depth=555 , 
	geological='地质层', water_elevation=333 , site_number= 'DF00000007', site_phone= '123234456', site_status= '使用', line_lenght= 666, 
	calibration= 888, location='安装位置简述'
where hole_number='DF2';


create view cdr_data_full as 
	select b.*, a.report_time, a.deepness, a.temperature, a.battery_level  
	from cdr_data as a, 
		cdr_site_info as b
	where a.site_number=b.site_number;






insert into cdr_site_info values(2, 'Z12', '1693345.234',  '1693345.234', '1693345.234', 123.321, 1111.234,  'dizhi', 234.321, 'DF000002', '13478900987', 'shoyong', 345.212, 1.234, 'location');

---插入某一条设备信息
insert cdr_site_info(hole_number, hole_coordinate_X, hole_coordinate_Y, hole_coordinate_Z, 
					hole_elevation, hole_depth, geological, water_elevation,
					site_number, site_phone, site_status, line_lenght, calibration, location)
	values('Z12', '1693345.234',  '1693345.234', '1693345.234', 123.321, 1111.234,  '地质', 234.321, 
	'DF00000006', '13478900987', '使用', 345.212, 1.234, '安装位置');

---查询设备所有信息
select hole_number, hole_coordinate_X, hole_coordinate_Y, hole_coordinate_Z, hole_elevation,hole_depth, geological, water_elevation, 
site_number, site_phone, site_status, line_lenght, calibration, location from cdr_site_info;

---删除某一条设备
delete from cdr_site_info where site_number='DF000002';




-- 筛选出每一组中时间最新的一条记录
-- 方法1
select a.* 
 from cdr_data_full a
 where not exists(select 1 
                  from cdr_data_full b
                  where b.site_number=a.site_number and b.report_time>a.report_time)
 
--方法2
select a.hole_number, a.hole_elevation, a.geological, a.report_time, a.deepness, a.hole_elevation-a.deepness as shuiWei,
	a.temperature, a.battery_level, a.site_number, a.site_phone, a.site_status 
from cdr_data_full as a
inner join
(
	select site_number, max(report_time) 'maxReport_time'
	from cdr_data_full 
	group by site_number
) b 
on a.site_number=b.site_number and a.report_time=b.maxReport_time;


---历史数据查询
select a.hole_number, a.geological, a.report_time, a.deepness, a.hole_elevation-a.deepness as shuiWei,
	a.temperature, a.battery_level, a.site_number, a.site_phone, a.site_status, a.location 
from cdr_data_full as a
where  a.report_time>'20180426170000' and a.report_time < ''


insert cdr_data(site_number, report_time, deepness, temperature, battery_level)
values('DF00000006', '1804011800', 23.33, 23.22, 81);

insert cdr_data(site_number, report_time, deepness, temperature, battery_level)
values('DF00000006', '1804031900', 34.33, 23.22, 81);

insert cdr_data(site_number, report_time, deepness, temperature, battery_level)
values('DF00000006', '1804051100', 44.33, 23.22, 81);

insert cdr_data(site_number, report_time, deepness, temperature, battery_level)
values('DF00000006', '1804101300', 12.33, 23.22, 81);

insert cdr_data(site_number, report_time, deepness, temperature, battery_level)
values('DF00000006', '1804151500', 22.33, 23.22, 81);

insert cdr_data(site_number, report_time, deepness, temperature, battery_level)
values('DF00000006', '18042121100', 77.33, 23.22, 81);


, min(deepness), min(temperature),avg(deepness), avg(temperature)
select max(deepness), max(temperature)
from cdr_data_full
where hole_number='Z12' and report_time like '201804%'order by report_time limit 10;


select max(deepness), max(temperature), min(deepness), min(temperature), avg(deepness), avg(temperature), min(battery_level), geological,hole_elevation 
from cdr_data_full  
where hole_number='Z12' and report_time like '2018-04%'



select max(deepness), 
		max(hole_elevation-deepness), 
		max(temperature), 
		min(deepness), 
		min(hole_elevation-deepness), 
		min(temperature), 
		round(avg(deepness), 3), 
		round(avg(hole_elevation-deepness), 3), 
		round(avg(temperature), 3), 
		min(battery_level) 
from cdr_data_full  
where report_time>'20180414223835' and report_time < '20180514223835' and hole_number='Z12'



