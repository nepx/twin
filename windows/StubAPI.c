
#include "Log.h"
#include "compat.h"

typedef struct {
	void  (*func)();
	char *fstr;
	char *lstr;
	int   ford;
} TRAPBLOCK;

#define TRAPS 1000

static void traphandler(int );
extern void debuggerbreak();

static void trap0()
{
  traphandler(0);
}

static void trap1()
{
  traphandler(1);
}

static void trap2()
{
  traphandler(2);
}

static void trap3()
{
  traphandler(3);
}

static void trap4()
{
  traphandler(4);
}

static void trap5()
{
  traphandler(5);
}

static void trap6()
{
  traphandler(6);
}

static void trap7()
{
  traphandler(7);
}

static void trap8()
{
  traphandler(8);
}

static void trap9()
{
  traphandler(9);
}

static void trap10()
{
  traphandler(10);
}

static void trap11()
{
  traphandler(11);
}

static void trap12()
{
  traphandler(12);
}

static void trap13()
{
  traphandler(13);
}

static void trap14()
{
  traphandler(14);
}

static void trap15()
{
  traphandler(15);
}

static void trap16()
{
  traphandler(16);
}

static void trap17()
{
  traphandler(17);
}

static void trap18()
{
  traphandler(18);
}

static void trap19()
{
  traphandler(19);
}

static void trap20()
{
  traphandler(20);
}

static void trap21()
{
  traphandler(21);
}

static void trap22()
{
  traphandler(22);
}

static void trap23()
{
  traphandler(23);
}

static void trap24()
{
  traphandler(24);
}

static void trap25()
{
  traphandler(25);
}

static void trap26()
{
  traphandler(26);
}

static void trap27()
{
  traphandler(27);
}

static void trap28()
{
  traphandler(28);
}

static void trap29()
{
  traphandler(29);
}

static void trap30()
{
  traphandler(30);
}

static void trap31()
{
  traphandler(31);
}

static void trap32()
{
  traphandler(32);
}

static void trap33()
{
  traphandler(33);
}

static void trap34()
{
  traphandler(34);
}

static void trap35()
{
  traphandler(35);
}

static void trap36()
{
  traphandler(36);
}

static void trap37()
{
  traphandler(37);
}

static void trap38()
{
  traphandler(38);
}

static void trap39()
{
  traphandler(39);
}

static void trap40()
{
  traphandler(40);
}

static void trap41()
{
  traphandler(41);
}

static void trap42()
{
  traphandler(42);
}

static void trap43()
{
  traphandler(43);
}

static void trap44()
{
  traphandler(44);
}

static void trap45()
{
  traphandler(45);
}

static void trap46()
{
  traphandler(46);
}

static void trap47()
{
  traphandler(47);
}

static void trap48()
{
  traphandler(48);
}

static void trap49()
{
  traphandler(49);
}

static void trap50()
{
  traphandler(50);
}

static void trap51()
{
  traphandler(51);
}

static void trap52()
{
  traphandler(52);
}

static void trap53()
{
  traphandler(53);
}

static void trap54()
{
  traphandler(54);
}

static void trap55()
{
  traphandler(55);
}

static void trap56()
{
  traphandler(56);
}

static void trap57()
{
  traphandler(57);
}

static void trap58()
{
  traphandler(58);
}

static void trap59()
{
  traphandler(59);
}

static void trap60()
{
  traphandler(60);
}

static void trap61()
{
  traphandler(61);
}

static void trap62()
{
  traphandler(62);
}

static void trap63()
{
  traphandler(63);
}

static void trap64()
{
  traphandler(64);
}

static void trap65()
{
  traphandler(65);
}

static void trap66()
{
  traphandler(66);
}

static void trap67()
{
  traphandler(67);
}

static void trap68()
{
  traphandler(68);
}

static void trap69()
{
  traphandler(69);
}

static void trap70()
{
  traphandler(70);
}

static void trap71()
{
  traphandler(71);
}

static void trap72()
{
  traphandler(72);
}

static void trap73()
{
  traphandler(73);
}

static void trap74()
{
  traphandler(74);
}

static void trap75()
{
  traphandler(75);
}

static void trap76()
{
  traphandler(76);
}

static void trap77()
{
  traphandler(77);
}

static void trap78()
{
  traphandler(78);
}

static void trap79()
{
  traphandler(79);
}

static void trap80()
{
  traphandler(80);
}

static void trap81()
{
  traphandler(81);
}

static void trap82()
{
  traphandler(82);
}

static void trap83()
{
  traphandler(83);
}

static void trap84()
{
  traphandler(84);
}

static void trap85()
{
  traphandler(85);
}

static void trap86()
{
  traphandler(86);
}

static void trap87()
{
  traphandler(87);
}

static void trap88()
{
  traphandler(88);
}

static void trap89()
{
  traphandler(89);
}

static void trap90()
{
  traphandler(90);
}

static void trap91()
{
  traphandler(91);
}

static void trap92()
{
  traphandler(92);
}

static void trap93()
{
  traphandler(93);
}

static void trap94()
{
  traphandler(94);
}

static void trap95()
{
  traphandler(95);
}

static void trap96()
{
  traphandler(96);
}

static void trap97()
{
  traphandler(97);
}

static void trap98()
{
  traphandler(98);
}

static void trap99()
{
  traphandler(99);
}

static void trap100()
{
  traphandler(100);
}

static void trap101()
{
  traphandler(101);
}

static void trap102()
{
  traphandler(102);
}

static void trap103()
{
  traphandler(103);
}

static void trap104()
{
  traphandler(104);
}

static void trap105()
{
  traphandler(105);
}

static void trap106()
{
  traphandler(106);
}

static void trap107()
{
  traphandler(107);
}

static void trap108()
{
  traphandler(108);
}

static void trap109()
{
  traphandler(109);
}

static void trap110()
{
  traphandler(110);
}

static void trap111()
{
  traphandler(111);
}

static void trap112()
{
  traphandler(112);
}

static void trap113()
{
  traphandler(113);
}

static void trap114()
{
  traphandler(114);
}

static void trap115()
{
  traphandler(115);
}

static void trap116()
{
  traphandler(116);
}

static void trap117()
{
  traphandler(117);
}

static void trap118()
{
  traphandler(118);
}

static void trap119()
{
  traphandler(119);
}

static void trap120()
{
  traphandler(120);
}

static void trap121()
{
  traphandler(121);
}

static void trap122()
{
  traphandler(122);
}

static void trap123()
{
  traphandler(123);
}

static void trap124()
{
  traphandler(124);
}

static void trap125()
{
  traphandler(125);
}

static void trap126()
{
  traphandler(126);
}

static void trap127()
{
  traphandler(127);
}

static void trap128()
{
  traphandler(128);
}

static void trap129()
{
  traphandler(129);
}

static void trap130()
{
  traphandler(130);
}

static void trap131()
{
  traphandler(131);
}

static void trap132()
{
  traphandler(132);
}

static void trap133()
{
  traphandler(133);
}

static void trap134()
{
  traphandler(134);
}

static void trap135()
{
  traphandler(135);
}

static void trap136()
{
  traphandler(136);
}

static void trap137()
{
  traphandler(137);
}

static void trap138()
{
  traphandler(138);
}

static void trap139()
{
  traphandler(139);
}

static void trap140()
{
  traphandler(140);
}

static void trap141()
{
  traphandler(141);
}

static void trap142()
{
  traphandler(142);
}

static void trap143()
{
  traphandler(143);
}

static void trap144()
{
  traphandler(144);
}

static void trap145()
{
  traphandler(145);
}

static void trap146()
{
  traphandler(146);
}

static void trap147()
{
  traphandler(147);
}

static void trap148()
{
  traphandler(148);
}

static void trap149()
{
  traphandler(149);
}

static void trap150()
{
  traphandler(150);
}

static void trap151()
{
  traphandler(151);
}

static void trap152()
{
  traphandler(152);
}

static void trap153()
{
  traphandler(153);
}

static void trap154()
{
  traphandler(154);
}

static void trap155()
{
  traphandler(155);
}

static void trap156()
{
  traphandler(156);
}

static void trap157()
{
  traphandler(157);
}

static void trap158()
{
  traphandler(158);
}

static void trap159()
{
  traphandler(159);
}

static void trap160()
{
  traphandler(160);
}

static void trap161()
{
  traphandler(161);
}

static void trap162()
{
  traphandler(162);
}

static void trap163()
{
  traphandler(163);
}

static void trap164()
{
  traphandler(164);
}

static void trap165()
{
  traphandler(165);
}

static void trap166()
{
  traphandler(166);
}

static void trap167()
{
  traphandler(167);
}

static void trap168()
{
  traphandler(168);
}

static void trap169()
{
  traphandler(169);
}

static void trap170()
{
  traphandler(170);
}

static void trap171()
{
  traphandler(171);
}

static void trap172()
{
  traphandler(172);
}

static void trap173()
{
  traphandler(173);
}

static void trap174()
{
  traphandler(174);
}

static void trap175()
{
  traphandler(175);
}

static void trap176()
{
  traphandler(176);
}

static void trap177()
{
  traphandler(177);
}

static void trap178()
{
  traphandler(178);
}

static void trap179()
{
  traphandler(179);
}

static void trap180()
{
  traphandler(180);
}

static void trap181()
{
  traphandler(181);
}

static void trap182()
{
  traphandler(182);
}

static void trap183()
{
  traphandler(183);
}

static void trap184()
{
  traphandler(184);
}

static void trap185()
{
  traphandler(185);
}

static void trap186()
{
  traphandler(186);
}

static void trap187()
{
  traphandler(187);
}

static void trap188()
{
  traphandler(188);
}

static void trap189()
{
  traphandler(189);
}

static void trap190()
{
  traphandler(190);
}

static void trap191()
{
  traphandler(191);
}

static void trap192()
{
  traphandler(192);
}

static void trap193()
{
  traphandler(193);
}

static void trap194()
{
  traphandler(194);
}

static void trap195()
{
  traphandler(195);
}

static void trap196()
{
  traphandler(196);
}

static void trap197()
{
  traphandler(197);
}

static void trap198()
{
  traphandler(198);
}

static void trap199()
{
  traphandler(199);
}

static void trap200()
{
  traphandler(200);
}

static void trap201()
{
  traphandler(201);
}

static void trap202()
{
  traphandler(202);
}

static void trap203()
{
  traphandler(203);
}

static void trap204()
{
  traphandler(204);
}

static void trap205()
{
  traphandler(205);
}

static void trap206()
{
  traphandler(206);
}

static void trap207()
{
  traphandler(207);
}

static void trap208()
{
  traphandler(208);
}

static void trap209()
{
  traphandler(209);
}

static void trap210()
{
  traphandler(210);
}

static void trap211()
{
  traphandler(211);
}

static void trap212()
{
  traphandler(212);
}

static void trap213()
{
  traphandler(213);
}

static void trap214()
{
  traphandler(214);
}

static void trap215()
{
  traphandler(215);
}

static void trap216()
{
  traphandler(216);
}

static void trap217()
{
  traphandler(217);
}

static void trap218()
{
  traphandler(218);
}

static void trap219()
{
  traphandler(219);
}

static void trap220()
{
  traphandler(220);
}

static void trap221()
{
  traphandler(221);
}

static void trap222()
{
  traphandler(222);
}

static void trap223()
{
  traphandler(223);
}

static void trap224()
{
  traphandler(224);
}

static void trap225()
{
  traphandler(225);
}

static void trap226()
{
  traphandler(226);
}

static void trap227()
{
  traphandler(227);
}

static void trap228()
{
  traphandler(228);
}

static void trap229()
{
  traphandler(229);
}

static void trap230()
{
  traphandler(230);
}

static void trap231()
{
  traphandler(231);
}

static void trap232()
{
  traphandler(232);
}

static void trap233()
{
  traphandler(233);
}

static void trap234()
{
  traphandler(234);
}

static void trap235()
{
  traphandler(235);
}

static void trap236()
{
  traphandler(236);
}

static void trap237()
{
  traphandler(237);
}

static void trap238()
{
  traphandler(238);
}

static void trap239()
{
  traphandler(239);
}

static void trap240()
{
  traphandler(240);
}

static void trap241()
{
  traphandler(241);
}

static void trap242()
{
  traphandler(242);
}

static void trap243()
{
  traphandler(243);
}

static void trap244()
{
  traphandler(244);
}

static void trap245()
{
  traphandler(245);
}

static void trap246()
{
  traphandler(246);
}

static void trap247()
{
  traphandler(247);
}

static void trap248()
{
  traphandler(248);
}

static void trap249()
{
  traphandler(249);
}

static void trap250()
{
  traphandler(250);
}

static void trap251()
{
  traphandler(251);
}

static void trap252()
{
  traphandler(252);
}

static void trap253()
{
  traphandler(253);
}

static void trap254()
{
  traphandler(254);
}

static void trap255()
{
  traphandler(255);
}

static void trap256()
{
  traphandler(256);
}

static void trap257()
{
  traphandler(257);
}

static void trap258()
{
  traphandler(258);
}

static void trap259()
{
  traphandler(259);
}

static void trap260()
{
  traphandler(260);
}

static void trap261()
{
  traphandler(261);
}

static void trap262()
{
  traphandler(262);
}

static void trap263()
{
  traphandler(263);
}

static void trap264()
{
  traphandler(264);
}

static void trap265()
{
  traphandler(265);
}

static void trap266()
{
  traphandler(266);
}

static void trap267()
{
  traphandler(267);
}

static void trap268()
{
  traphandler(268);
}

static void trap269()
{
  traphandler(269);
}

static void trap270()
{
  traphandler(270);
}

static void trap271()
{
  traphandler(271);
}

static void trap272()
{
  traphandler(272);
}

static void trap273()
{
  traphandler(273);
}

static void trap274()
{
  traphandler(274);
}

static void trap275()
{
  traphandler(275);
}

static void trap276()
{
  traphandler(276);
}

static void trap277()
{
  traphandler(277);
}

static void trap278()
{
  traphandler(278);
}

static void trap279()
{
  traphandler(279);
}

static void trap280()
{
  traphandler(280);
}

static void trap281()
{
  traphandler(281);
}

static void trap282()
{
  traphandler(282);
}

static void trap283()
{
  traphandler(283);
}

static void trap284()
{
  traphandler(284);
}

static void trap285()
{
  traphandler(285);
}

static void trap286()
{
  traphandler(286);
}

static void trap287()
{
  traphandler(287);
}

static void trap288()
{
  traphandler(288);
}

static void trap289()
{
  traphandler(289);
}

static void trap290()
{
  traphandler(290);
}

static void trap291()
{
  traphandler(291);
}

static void trap292()
{
  traphandler(292);
}

static void trap293()
{
  traphandler(293);
}

static void trap294()
{
  traphandler(294);
}

static void trap295()
{
  traphandler(295);
}

static void trap296()
{
  traphandler(296);
}

static void trap297()
{
  traphandler(297);
}

static void trap298()
{
  traphandler(298);
}

static void trap299()
{
  traphandler(299);
}

static void trap300()
{
  traphandler(300);
}

static void trap301()
{
  traphandler(301);
}

static void trap302()
{
  traphandler(302);
}

static void trap303()
{
  traphandler(303);
}

static void trap304()
{
  traphandler(304);
}

static void trap305()
{
  traphandler(305);
}

static void trap306()
{
  traphandler(306);
}

static void trap307()
{
  traphandler(307);
}

static void trap308()
{
  traphandler(308);
}

static void trap309()
{
  traphandler(309);
}

static void trap310()
{
  traphandler(310);
}

static void trap311()
{
  traphandler(311);
}

static void trap312()
{
  traphandler(312);
}

static void trap313()
{
  traphandler(313);
}

static void trap314()
{
  traphandler(314);
}

static void trap315()
{
  traphandler(315);
}

static void trap316()
{
  traphandler(316);
}

static void trap317()
{
  traphandler(317);
}

static void trap318()
{
  traphandler(318);
}

static void trap319()
{
  traphandler(319);
}

static void trap320()
{
  traphandler(320);
}

static void trap321()
{
  traphandler(321);
}

static void trap322()
{
  traphandler(322);
}

static void trap323()
{
  traphandler(323);
}

static void trap324()
{
  traphandler(324);
}

static void trap325()
{
  traphandler(325);
}

static void trap326()
{
  traphandler(326);
}

static void trap327()
{
  traphandler(327);
}

static void trap328()
{
  traphandler(328);
}

static void trap329()
{
  traphandler(329);
}

static void trap330()
{
  traphandler(330);
}

static void trap331()
{
  traphandler(331);
}

static void trap332()
{
  traphandler(332);
}

static void trap333()
{
  traphandler(333);
}

static void trap334()
{
  traphandler(334);
}

static void trap335()
{
  traphandler(335);
}

static void trap336()
{
  traphandler(336);
}

static void trap337()
{
  traphandler(337);
}

static void trap338()
{
  traphandler(338);
}

static void trap339()
{
  traphandler(339);
}

static void trap340()
{
  traphandler(340);
}

static void trap341()
{
  traphandler(341);
}

static void trap342()
{
  traphandler(342);
}

static void trap343()
{
  traphandler(343);
}

static void trap344()
{
  traphandler(344);
}

static void trap345()
{
  traphandler(345);
}

static void trap346()
{
  traphandler(346);
}

static void trap347()
{
  traphandler(347);
}

static void trap348()
{
  traphandler(348);
}

static void trap349()
{
  traphandler(349);
}

static void trap350()
{
  traphandler(350);
}

static void trap351()
{
  traphandler(351);
}

static void trap352()
{
  traphandler(352);
}

static void trap353()
{
  traphandler(353);
}

static void trap354()
{
  traphandler(354);
}

static void trap355()
{
  traphandler(355);
}

static void trap356()
{
  traphandler(356);
}

static void trap357()
{
  traphandler(357);
}

static void trap358()
{
  traphandler(358);
}

static void trap359()
{
  traphandler(359);
}

static void trap360()
{
  traphandler(360);
}

static void trap361()
{
  traphandler(361);
}

static void trap362()
{
  traphandler(362);
}

static void trap363()
{
  traphandler(363);
}

static void trap364()
{
  traphandler(364);
}

static void trap365()
{
  traphandler(365);
}

static void trap366()
{
  traphandler(366);
}

static void trap367()
{
  traphandler(367);
}

static void trap368()
{
  traphandler(368);
}

static void trap369()
{
  traphandler(369);
}

static void trap370()
{
  traphandler(370);
}

static void trap371()
{
  traphandler(371);
}

static void trap372()
{
  traphandler(372);
}

static void trap373()
{
  traphandler(373);
}

static void trap374()
{
  traphandler(374);
}

static void trap375()
{
  traphandler(375);
}

static void trap376()
{
  traphandler(376);
}

static void trap377()
{
  traphandler(377);
}

static void trap378()
{
  traphandler(378);
}

static void trap379()
{
  traphandler(379);
}

static void trap380()
{
  traphandler(380);
}

static void trap381()
{
  traphandler(381);
}

static void trap382()
{
  traphandler(382);
}

static void trap383()
{
  traphandler(383);
}

static void trap384()
{
  traphandler(384);
}

static void trap385()
{
  traphandler(385);
}

static void trap386()
{
  traphandler(386);
}

static void trap387()
{
  traphandler(387);
}

static void trap388()
{
  traphandler(388);
}

static void trap389()
{
  traphandler(389);
}

static void trap390()
{
  traphandler(390);
}

static void trap391()
{
  traphandler(391);
}

static void trap392()
{
  traphandler(392);
}

static void trap393()
{
  traphandler(393);
}

static void trap394()
{
  traphandler(394);
}

static void trap395()
{
  traphandler(395);
}

static void trap396()
{
  traphandler(396);
}

static void trap397()
{
  traphandler(397);
}

static void trap398()
{
  traphandler(398);
}

static void trap399()
{
  traphandler(399);
}

static void trap400()
{
  traphandler(400);
}

static void trap401()
{
  traphandler(401);
}

static void trap402()
{
  traphandler(402);
}

static void trap403()
{
  traphandler(403);
}

static void trap404()
{
  traphandler(404);
}

static void trap405()
{
  traphandler(405);
}

static void trap406()
{
  traphandler(406);
}

static void trap407()
{
  traphandler(407);
}

static void trap408()
{
  traphandler(408);
}

static void trap409()
{
  traphandler(409);
}

static void trap410()
{
  traphandler(410);
}

static void trap411()
{
  traphandler(411);
}

static void trap412()
{
  traphandler(412);
}

static void trap413()
{
  traphandler(413);
}

static void trap414()
{
  traphandler(414);
}

static void trap415()
{
  traphandler(415);
}

static void trap416()
{
  traphandler(416);
}

static void trap417()
{
  traphandler(417);
}

static void trap418()
{
  traphandler(418);
}

static void trap419()
{
  traphandler(419);
}

static void trap420()
{
  traphandler(420);
}

static void trap421()
{
  traphandler(421);
}

static void trap422()
{
  traphandler(422);
}

static void trap423()
{
  traphandler(423);
}

static void trap424()
{
  traphandler(424);
}

static void trap425()
{
  traphandler(425);
}

static void trap426()
{
  traphandler(426);
}

static void trap427()
{
  traphandler(427);
}

static void trap428()
{
  traphandler(428);
}

static void trap429()
{
  traphandler(429);
}

static void trap430()
{
  traphandler(430);
}

static void trap431()
{
  traphandler(431);
}

static void trap432()
{
  traphandler(432);
}

static void trap433()
{
  traphandler(433);
}

static void trap434()
{
  traphandler(434);
}

static void trap435()
{
  traphandler(435);
}

static void trap436()
{
  traphandler(436);
}

static void trap437()
{
  traphandler(437);
}

static void trap438()
{
  traphandler(438);
}

static void trap439()
{
  traphandler(439);
}

static void trap440()
{
  traphandler(440);
}

static void trap441()
{
  traphandler(441);
}

static void trap442()
{
  traphandler(442);
}

static void trap443()
{
  traphandler(443);
}

static void trap444()
{
  traphandler(444);
}

static void trap445()
{
  traphandler(445);
}

static void trap446()
{
  traphandler(446);
}

static void trap447()
{
  traphandler(447);
}

static void trap448()
{
  traphandler(448);
}

static void trap449()
{
  traphandler(449);
}

static void trap450()
{
  traphandler(450);
}

static void trap451()
{
  traphandler(451);
}

static void trap452()
{
  traphandler(452);
}

static void trap453()
{
  traphandler(453);
}

static void trap454()
{
  traphandler(454);
}

static void trap455()
{
  traphandler(455);
}

static void trap456()
{
  traphandler(456);
}

static void trap457()
{
  traphandler(457);
}

static void trap458()
{
  traphandler(458);
}

static void trap459()
{
  traphandler(459);
}

static void trap460()
{
  traphandler(460);
}

static void trap461()
{
  traphandler(461);
}

static void trap462()
{
  traphandler(462);
}

static void trap463()
{
  traphandler(463);
}

static void trap464()
{
  traphandler(464);
}

static void trap465()
{
  traphandler(465);
}

static void trap466()
{
  traphandler(466);
}

static void trap467()
{
  traphandler(467);
}

static void trap468()
{
  traphandler(468);
}

static void trap469()
{
  traphandler(469);
}

static void trap470()
{
  traphandler(470);
}

static void trap471()
{
  traphandler(471);
}

static void trap472()
{
  traphandler(472);
}

static void trap473()
{
  traphandler(473);
}

static void trap474()
{
  traphandler(474);
}

static void trap475()
{
  traphandler(475);
}

static void trap476()
{
  traphandler(476);
}

static void trap477()
{
  traphandler(477);
}

static void trap478()
{
  traphandler(478);
}

static void trap479()
{
  traphandler(479);
}

static void trap480()
{
  traphandler(480);
}

static void trap481()
{
  traphandler(481);
}

static void trap482()
{
  traphandler(482);
}

static void trap483()
{
  traphandler(483);
}

static void trap484()
{
  traphandler(484);
}

static void trap485()
{
  traphandler(485);
}

static void trap486()
{
  traphandler(486);
}

static void trap487()
{
  traphandler(487);
}

static void trap488()
{
  traphandler(488);
}

static void trap489()
{
  traphandler(489);
}

static void trap490()
{
  traphandler(490);
}

static void trap491()
{
  traphandler(491);
}

static void trap492()
{
  traphandler(492);
}

static void trap493()
{
  traphandler(493);
}

static void trap494()
{
  traphandler(494);
}

static void trap495()
{
  traphandler(495);
}

static void trap496()
{
  traphandler(496);
}

static void trap497()
{
  traphandler(497);
}

static void trap498()
{
  traphandler(498);
}

static void trap499()
{
  traphandler(499);
}

TRAPBLOCK wintraps[TRAPS] = { 
{ trap0,0,0 },
{ trap1,0,0 },
{ trap2,0,0 },
{ trap3,0,0 },
{ trap4,0,0 },
{ trap5,0,0 },
{ trap6,0,0 },
{ trap7,0,0 },
{ trap8,0,0 },
{ trap9,0,0 },
{ trap10,0,0 },
{ trap11,0,0 },
{ trap12,0,0 },
{ trap13,0,0 },
{ trap14,0,0 },
{ trap15,0,0 },
{ trap16,0,0 },
{ trap17,0,0 },
{ trap18,0,0 },
{ trap19,0,0 },
{ trap20,0,0 },
{ trap21,0,0 },
{ trap22,0,0 },
{ trap23,0,0 },
{ trap24,0,0 },
{ trap25,0,0 },
{ trap26,0,0 },
{ trap27,0,0 },
{ trap28,0,0 },
{ trap29,0,0 },
{ trap30,0,0 },
{ trap31,0,0 },
{ trap32,0,0 },
{ trap33,0,0 },
{ trap34,0,0 },
{ trap35,0,0 },
{ trap36,0,0 },
{ trap37,0,0 },
{ trap38,0,0 },
{ trap39,0,0 },
{ trap40,0,0 },
{ trap41,0,0 },
{ trap42,0,0 },
{ trap43,0,0 },
{ trap44,0,0 },
{ trap45,0,0 },
{ trap46,0,0 },
{ trap47,0,0 },
{ trap48,0,0 },
{ trap49,0,0 },
{ trap50,0,0 },
{ trap51,0,0 },
{ trap52,0,0 },
{ trap53,0,0 },
{ trap54,0,0 },
{ trap55,0,0 },
{ trap56,0,0 },
{ trap57,0,0 },
{ trap58,0,0 },
{ trap59,0,0 },
{ trap60,0,0 },
{ trap61,0,0 },
{ trap62,0,0 },
{ trap63,0,0 },
{ trap64,0,0 },
{ trap65,0,0 },
{ trap66,0,0 },
{ trap67,0,0 },
{ trap68,0,0 },
{ trap69,0,0 },
{ trap70,0,0 },
{ trap71,0,0 },
{ trap72,0,0 },
{ trap73,0,0 },
{ trap74,0,0 },
{ trap75,0,0 },
{ trap76,0,0 },
{ trap77,0,0 },
{ trap78,0,0 },
{ trap79,0,0 },
{ trap80,0,0 },
{ trap81,0,0 },
{ trap82,0,0 },
{ trap83,0,0 },
{ trap84,0,0 },
{ trap85,0,0 },
{ trap86,0,0 },
{ trap87,0,0 },
{ trap88,0,0 },
{ trap89,0,0 },
{ trap90,0,0 },
{ trap91,0,0 },
{ trap92,0,0 },
{ trap93,0,0 },
{ trap94,0,0 },
{ trap95,0,0 },
{ trap96,0,0 },
{ trap97,0,0 },
{ trap98,0,0 },
{ trap99,0,0 },
{ trap100,0,0 },
{ trap101,0,0 },
{ trap102,0,0 },
{ trap103,0,0 },
{ trap104,0,0 },
{ trap105,0,0 },
{ trap106,0,0 },
{ trap107,0,0 },
{ trap108,0,0 },
{ trap109,0,0 },
{ trap110,0,0 },
{ trap111,0,0 },
{ trap112,0,0 },
{ trap113,0,0 },
{ trap114,0,0 },
{ trap115,0,0 },
{ trap116,0,0 },
{ trap117,0,0 },
{ trap118,0,0 },
{ trap119,0,0 },
{ trap120,0,0 },
{ trap121,0,0 },
{ trap122,0,0 },
{ trap123,0,0 },
{ trap124,0,0 },
{ trap125,0,0 },
{ trap126,0,0 },
{ trap127,0,0 },
{ trap128,0,0 },
{ trap129,0,0 },
{ trap130,0,0 },
{ trap131,0,0 },
{ trap132,0,0 },
{ trap133,0,0 },
{ trap134,0,0 },
{ trap135,0,0 },
{ trap136,0,0 },
{ trap137,0,0 },
{ trap138,0,0 },
{ trap139,0,0 },
{ trap140,0,0 },
{ trap141,0,0 },
{ trap142,0,0 },
{ trap143,0,0 },
{ trap144,0,0 },
{ trap145,0,0 },
{ trap146,0,0 },
{ trap147,0,0 },
{ trap148,0,0 },
{ trap149,0,0 },
{ trap150,0,0 },
{ trap151,0,0 },
{ trap152,0,0 },
{ trap153,0,0 },
{ trap154,0,0 },
{ trap155,0,0 },
{ trap156,0,0 },
{ trap157,0,0 },
{ trap158,0,0 },
{ trap159,0,0 },
{ trap160,0,0 },
{ trap161,0,0 },
{ trap162,0,0 },
{ trap163,0,0 },
{ trap164,0,0 },
{ trap165,0,0 },
{ trap166,0,0 },
{ trap167,0,0 },
{ trap168,0,0 },
{ trap169,0,0 },
{ trap170,0,0 },
{ trap171,0,0 },
{ trap172,0,0 },
{ trap173,0,0 },
{ trap174,0,0 },
{ trap175,0,0 },
{ trap176,0,0 },
{ trap177,0,0 },
{ trap178,0,0 },
{ trap179,0,0 },
{ trap180,0,0 },
{ trap181,0,0 },
{ trap182,0,0 },
{ trap183,0,0 },
{ trap184,0,0 },
{ trap185,0,0 },
{ trap186,0,0 },
{ trap187,0,0 },
{ trap188,0,0 },
{ trap189,0,0 },
{ trap190,0,0 },
{ trap191,0,0 },
{ trap192,0,0 },
{ trap193,0,0 },
{ trap194,0,0 },
{ trap195,0,0 },
{ trap196,0,0 },
{ trap197,0,0 },
{ trap198,0,0 },
{ trap199,0,0 },
{ trap200,0,0 },
{ trap201,0,0 },
{ trap202,0,0 },
{ trap203,0,0 },
{ trap204,0,0 },
{ trap205,0,0 },
{ trap206,0,0 },
{ trap207,0,0 },
{ trap208,0,0 },
{ trap209,0,0 },
{ trap210,0,0 },
{ trap211,0,0 },
{ trap212,0,0 },
{ trap213,0,0 },
{ trap214,0,0 },
{ trap215,0,0 },
{ trap216,0,0 },
{ trap217,0,0 },
{ trap218,0,0 },
{ trap219,0,0 },
{ trap220,0,0 },
{ trap221,0,0 },
{ trap222,0,0 },
{ trap223,0,0 },
{ trap224,0,0 },
{ trap225,0,0 },
{ trap226,0,0 },
{ trap227,0,0 },
{ trap228,0,0 },
{ trap229,0,0 },
{ trap230,0,0 },
{ trap231,0,0 },
{ trap232,0,0 },
{ trap233,0,0 },
{ trap234,0,0 },
{ trap235,0,0 },
{ trap236,0,0 },
{ trap237,0,0 },
{ trap238,0,0 },
{ trap239,0,0 },
{ trap240,0,0 },
{ trap241,0,0 },
{ trap242,0,0 },
{ trap243,0,0 },
{ trap244,0,0 },
{ trap245,0,0 },
{ trap246,0,0 },
{ trap247,0,0 },
{ trap248,0,0 },
{ trap249,0,0 },
{ trap250,0,0 },
{ trap251,0,0 },
{ trap252,0,0 },
{ trap253,0,0 },
{ trap254,0,0 },
{ trap255,0,0 },
{ trap256,0,0 },
{ trap257,0,0 },
{ trap258,0,0 },
{ trap259,0,0 },
{ trap260,0,0 },
{ trap261,0,0 },
{ trap262,0,0 },
{ trap263,0,0 },
{ trap264,0,0 },
{ trap265,0,0 },
{ trap266,0,0 },
{ trap267,0,0 },
{ trap268,0,0 },
{ trap269,0,0 },
{ trap270,0,0 },
{ trap271,0,0 },
{ trap272,0,0 },
{ trap273,0,0 },
{ trap274,0,0 },
{ trap275,0,0 },
{ trap276,0,0 },
{ trap277,0,0 },
{ trap278,0,0 },
{ trap279,0,0 },
{ trap280,0,0 },
{ trap281,0,0 },
{ trap282,0,0 },
{ trap283,0,0 },
{ trap284,0,0 },
{ trap285,0,0 },
{ trap286,0,0 },
{ trap287,0,0 },
{ trap288,0,0 },
{ trap289,0,0 },
{ trap290,0,0 },
{ trap291,0,0 },
{ trap292,0,0 },
{ trap293,0,0 },
{ trap294,0,0 },
{ trap295,0,0 },
{ trap296,0,0 },
{ trap297,0,0 },
{ trap298,0,0 },
{ trap299,0,0 },
{ trap300,0,0 },
{ trap301,0,0 },
{ trap302,0,0 },
{ trap303,0,0 },
{ trap304,0,0 },
{ trap305,0,0 },
{ trap306,0,0 },
{ trap307,0,0 },
{ trap308,0,0 },
{ trap309,0,0 },
{ trap310,0,0 },
{ trap311,0,0 },
{ trap312,0,0 },
{ trap313,0,0 },
{ trap314,0,0 },
{ trap315,0,0 },
{ trap316,0,0 },
{ trap317,0,0 },
{ trap318,0,0 },
{ trap319,0,0 },
{ trap320,0,0 },
{ trap321,0,0 },
{ trap322,0,0 },
{ trap323,0,0 },
{ trap324,0,0 },
{ trap325,0,0 },
{ trap326,0,0 },
{ trap327,0,0 },
{ trap328,0,0 },
{ trap329,0,0 },
{ trap330,0,0 },
{ trap331,0,0 },
{ trap332,0,0 },
{ trap333,0,0 },
{ trap334,0,0 },
{ trap335,0,0 },
{ trap336,0,0 },
{ trap337,0,0 },
{ trap338,0,0 },
{ trap339,0,0 },
{ trap340,0,0 },
{ trap341,0,0 },
{ trap342,0,0 },
{ trap343,0,0 },
{ trap344,0,0 },
{ trap345,0,0 },
{ trap346,0,0 },
{ trap347,0,0 },
{ trap348,0,0 },
{ trap349,0,0 },
{ trap350,0,0 },
{ trap351,0,0 },
{ trap352,0,0 },
{ trap353,0,0 },
{ trap354,0,0 },
{ trap355,0,0 },
{ trap356,0,0 },
{ trap357,0,0 },
{ trap358,0,0 },
{ trap359,0,0 },
{ trap360,0,0 },
{ trap361,0,0 },
{ trap362,0,0 },
{ trap363,0,0 },
{ trap364,0,0 },
{ trap365,0,0 },
{ trap366,0,0 },
{ trap367,0,0 },
{ trap368,0,0 },
{ trap369,0,0 },
{ trap370,0,0 },
{ trap371,0,0 },
{ trap372,0,0 },
{ trap373,0,0 },
{ trap374,0,0 },
{ trap375,0,0 },
{ trap376,0,0 },
{ trap377,0,0 },
{ trap378,0,0 },
{ trap379,0,0 },
{ trap380,0,0 },
{ trap381,0,0 },
{ trap382,0,0 },
{ trap383,0,0 },
{ trap384,0,0 },
{ trap385,0,0 },
{ trap386,0,0 },
{ trap387,0,0 },
{ trap388,0,0 },
{ trap389,0,0 },
{ trap390,0,0 },
{ trap391,0,0 },
{ trap392,0,0 },
{ trap393,0,0 },
{ trap394,0,0 },
{ trap395,0,0 },
{ trap396,0,0 },
{ trap397,0,0 },
{ trap398,0,0 },
{ trap399,0,0 },
{ trap400,0,0 },
{ trap401,0,0 },
{ trap402,0,0 },
{ trap403,0,0 },
{ trap404,0,0 },
{ trap405,0,0 },
{ trap406,0,0 },
{ trap407,0,0 },
{ trap408,0,0 },
{ trap409,0,0 },
{ trap410,0,0 },
{ trap411,0,0 },
{ trap412,0,0 },
{ trap413,0,0 },
{ trap414,0,0 },
{ trap415,0,0 },
{ trap416,0,0 },
{ trap417,0,0 },
{ trap418,0,0 },
{ trap419,0,0 },
{ trap420,0,0 },
{ trap421,0,0 },
{ trap422,0,0 },
{ trap423,0,0 },
{ trap424,0,0 },
{ trap425,0,0 },
{ trap426,0,0 },
{ trap427,0,0 },
{ trap428,0,0 },
{ trap429,0,0 },
{ trap430,0,0 },
{ trap431,0,0 },
{ trap432,0,0 },
{ trap433,0,0 },
{ trap434,0,0 },
{ trap435,0,0 },
{ trap436,0,0 },
{ trap437,0,0 },
{ trap438,0,0 },
{ trap439,0,0 },
{ trap440,0,0 },
{ trap441,0,0 },
{ trap442,0,0 },
{ trap443,0,0 },
{ trap444,0,0 },
{ trap445,0,0 },
{ trap446,0,0 },
{ trap447,0,0 },
{ trap448,0,0 },
{ trap449,0,0 },
{ trap450,0,0 },
{ trap451,0,0 },
{ trap452,0,0 },
{ trap453,0,0 },
{ trap454,0,0 },
{ trap455,0,0 },
{ trap456,0,0 },
{ trap457,0,0 },
{ trap458,0,0 },
{ trap459,0,0 },
{ trap460,0,0 },
{ trap461,0,0 },
{ trap462,0,0 },
{ trap463,0,0 },
{ trap464,0,0 },
{ trap465,0,0 },
{ trap466,0,0 },
{ trap467,0,0 },
{ trap468,0,0 },
{ trap469,0,0 },
{ trap470,0,0 },
{ trap471,0,0 },
{ trap472,0,0 },
{ trap473,0,0 },
{ trap474,0,0 },
{ trap475,0,0 },
{ trap476,0,0 },
{ trap477,0,0 },
{ trap478,0,0 },
{ trap479,0,0 },
{ trap480,0,0 },
{ trap481,0,0 },
{ trap482,0,0 },
{ trap483,0,0 },
{ trap484,0,0 },
{ trap485,0,0 },
{ trap486,0,0 },
{ trap487,0,0 },
{ trap488,0,0 },
{ trap489,0,0 },
{ trap490,0,0 },
{ trap491,0,0 },
{ trap492,0,0 },
{ trap493,0,0 },
{ trap494,0,0 },
{ trap495,0,0 },
{ trap496,0,0 },
{ trap497,0,0 },
{ trap498,0,0 },
{ trap499,0,0 },

};

static int trapcount;

static void traphandler(int id)
{
	char buffer[256];

	logstr(LF_CONSOLE,"Trapped unimplemented TRAP %s %s:%d\n",
        	wintraps[id].fstr,
        	wintraps[id].lstr,
		wintraps[id].ford);

	wsprintf(buffer,"Unimplemented interface function\n%s:%d\n%s",
		wintraps[id].lstr,
		wintraps[id].ford,
		wintraps[id].fstr);
		
	if (GetConfigOption(WD_APISTUBS)) {
		MessageBox((HWND)NULL,buffer,
			"LoadPE load error.",
			MB_OK);
	}

	FatalAppExit(0,"Unimplemented bin32 interface functions.\n");
}

int Win32Trap(long);

DWORD
WinStubAPI(char *libstr,char *lpstr,int id)
{
  if(trapcount == TRAPS) {
	logstr(LF_CONSOLE,"Trap buffer exceeded %s %d\n",
		lpstr,id);
	return (DWORD) Win32Trap;
  }

  wintraps[trapcount].fstr = strdup(lpstr);
  wintraps[trapcount].lstr = strdup(libstr);
  wintraps[trapcount].ford = id;
  return (DWORD) wintraps[trapcount++].func;
}

extern void debuggerbreak();
int
Win32Trap(long bogus)
{
	long *backtrack;
	
	/* this is the return address of the caller */
	backtrack = &bogus;
	backtrack--;

	/* now backup to the address in the previous instruction */
	backtrack = (long *) *backtrack;
	backtrack--;

	logstr(LF_CONSOLE,"Win32 Code Segment Trap %p\n",*backtrack);
	debuggerbreak();
	return 0;
}
