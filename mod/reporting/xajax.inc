<?php
/**
 * @name         Report Generation System
 * @version      0.1
 * @package      reporting
 * @author       Milindu Sanoj Kumarage <agentmilindu@gmail.com>
 * @about        Developed in whole or part by the U.S. National Library of Medicine and the Sahana Foundation
 * @link         https://pl.nlm.nih.gov/about
 * @link         http://sahanafoundation.org
 * @link		 http://www.agentmilindu.com
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2014.07.08
 */

global $global;

// create an array as it does not exist previosuly and will make warnings
$global['xajax_functions'] = array();

// publicly register accessible xajax funtions
array_push($global['xajax_functions'],'reporting_create_new');




//Maps
function reporting_create_new() {
	global $global;
	
	$global['xajax_res']->addAppend('rezLog','innerHTML',$message);
	$global['xajax_res']->addScript("");
	return $global['xajax_res']->getXML();
}
function reporting_create_new2($name) {
	global $global;
	$q = "
		INSERT INTO map(name,thumbnail)
		VALUES ('$name','http://placehold.it/150x150');
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "Inserting map"); }
	$global['xajax_res']->addScript("window.location='?add&tabid=1&map_id=".mysql_insert_id()."&name=$name'");
	return $global['xajax_res']->getXML();
}
function shn_text_reporting_create() {
global $global;
require_once($global['approot'] . '/3rd/tcpdf/tcpdf.php');
	
// create new PDF document
$pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);

// set document information
$pdf->SetCreator(PDF_CREATOR);
$pdf->SetAuthor('Sahana DMU');
$pdf->SetTitle('Sahana DMU 001');
$pdf->SetSubject('Sahana DMU Report');
$pdf->SetKeywords('Sahana DMU, PDF, example, test, guide');

// set default header data
$pdf->SetHeaderData(PDF_HEADER_LOGO, PDF_HEADER_LOGO_WIDTH, PDF_HEADER_TITLE, PDF_HEADER_STRING, array(0,64,255), array(0,64,128));
$pdf->setFooterData(array(0,64,0), array(0,64,128));

// set header and footer fonts
$pdf->setHeaderFont(Array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
$pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));

// set default monospaced font
$pdf->SetDefaultMonospacedFont(PDF_FONT_MONOSPACED);

// set margins
$pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
$pdf->SetHeaderMargin(PDF_MARGIN_HEADER);
$pdf->SetFooterMargin(PDF_MARGIN_FOOTER);

// set auto page breaks
$pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

// set image scale factor
$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO);

// set some language-dependent strings (optional)
if (@file_exists(dirname(__FILE__).'/lang/eng.php')) {
    require_once(dirname(__FILE__).'/lang/eng.php');
    $pdf->setLanguageArray($l);
}

// ---------------------------------------------------------

// set default font subsetting mode
$pdf->setFontSubsetting(true);

// Set font
// dejavusans is a UTF-8 Unicode font, if you only need to
// print standard ASCII chars, you can use core fonts like
// helvetica or times to reduce file size.
$pdf->SetFont('dejavusans', '', 14, '', true);

// Add a page
// This method has several options, check the source code documentation for more information.
$pdf->AddPage();

// set text shadow effect
$pdf->setTextShadow(array('enabled'=>true, 'depth_w'=>0.2, 'depth_h'=>0.2, 'color'=>array(196,196,196), 'opacity'=>1, 'blend_mode'=>'Normal'));

// Set some content to print
$html = <<<EOD
<h1>Welcome to Sahana DMU</h1>
<i>This is the first example of Sahana DMU reporting.</i>
EOD;

$q = "
	SELECT * from map;
";
$result = $global['db']->Execute($q);
if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "show events 1"); }
$html .="<ul>";
while ($row = $result->FetchRow()) {
            $html .= "<li > " .$row['name']."</li>";
}
$html .="</ul>";

// Print text using writeHTMLCell()
$pdf->writeHTMLCell(0, 0, '', '', $html, 0, 1, 0, true, '', true);

// ---------------------------------------------------------

// Close and output PDF document
// This method has several options, check the source code documentation for more information.
$pdf->Output('example_001.pdf', 'I');

//============================================================+
// END OF FILE
//============================================================+
}
function shn_text_reporting_dar() {
global $global;
require_once($global['approot'] . '/3rd/tcpdf/tcpdf.php');
	
// create new PDF document
$pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);

// set document information
$pdf->SetCreator(PDF_CREATOR);
$pdf->SetAuthor('Sahana DMU');
$pdf->SetTitle('Sahana DMU 001');
$pdf->SetSubject('Sahana DMU Report');
$pdf->SetKeywords('Sahana DMU, PDF, example, test, guide');

// set default header data
$pdf->SetHeaderData(PDF_HEADER_LOGO, PDF_HEADER_LOGO_WIDTH, PDF_HEADER_TITLE, PDF_HEADER_STRING, array(0,64,255), array(0,64,128));
$pdf->setFooterData(array(0,64,0), array(0,64,128));

// set header and footer fonts
$pdf->setHeaderFont(Array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
$pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));

// set default monospaced font
$pdf->SetDefaultMonospacedFont(PDF_FONT_MONOSPACED);

// set margins
$pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
$pdf->SetHeaderMargin(PDF_MARGIN_HEADER);
$pdf->SetFooterMargin(PDF_MARGIN_FOOTER);

// set auto page breaks
$pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

// set image scale factor
$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO);

// set some language-dependent strings (optional)
if (@file_exists(dirname(__FILE__).'/lang/eng.php')) {
    require_once(dirname(__FILE__).'/lang/eng.php');
    $pdf->setLanguageArray($l);
}

// ---------------------------------------------------------

// set default font subsetting mode
$pdf->setFontSubsetting(true);

// Set font
// dejavusans is a UTF-8 Unicode font, if you only need to
// print standard ASCII chars, you can use core fonts like
// helvetica or times to reduce file size.
$pdf->SetFont('times', '', 14, '', true);

// Add a page
// This method has several options, check the source code documentation for more information.
$pdf->AddPage();

// set text shadow effect
$pdf->setTextShadow(array('enabled'=>true, 'depth_w'=>0.2, 'depth_h'=>0.2, 'color'=>array(196,196,196), 'opacity'=>1, 'blend_mode'=>'Normal'));

// Set some content to print
$html = <<<EOD
<h2>Disaster Assessment and Response</h1>
<i>Water and Sanitation Health</i>
EOD;

$q = "
	SELECT * from WASH;
";
$result = $global['db']->Execute($q);
if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "show events 1"); }
$html .="<div>";
while ($row = $result->FetchRow()) {
            $html .= "<hr/>";
            $html .= "<h4 >Number of water sources</h4><ul>";
            $html .= "<li >Within 500m: ".$row['water_in500']."</li>";
            $html .= "<li >Further than 500m: ".$row['water_out500']."</li>";
			$html .= "<li >Require water treatment: ";
			$html .=($row['need_treatment']==1)?"Yes":"No"."</li>";
			$html .= "<li >Estimated Requirement of Water/day:  ".$row['water_per_day']."</li></ul>";
			
            $html .= "<h4 >Estimated waiting time at a single water source</h4><ul>";
            $html .= "<li >Pipe-borne: ".$row['wait_pipe_borne']."h</li>";
            $html .= "<li >Open Well: ".$row['wait_open_well']."h</li>";
            $html .= "<li >Hand-pump: ".$row['wait_hand_pump']."h</li>";
			$html .= "<li >Require basic hygiene training: ";
			$html .=($row['need_hygin_training']==1)?"Yes":"No";
			$html .="</li></ul>";
			
			$html .= "<h4 >Toilets</h4><ul>";
            $html .= "<li >Number of appropriately designed toilets: ".$row['toilets']."</li>";
            $html .= "<li >Required Number: ".$row['required_toilets']."</li>";
            $html .= "<li >Deficit: ".$row['deficit_toilet']."</li></ul>";
}
$html .="</div>";

// Print text using writeHTMLCell()
$pdf->writeHTMLCell(0, 0, '', '', $html, 0, 1, 0, true, '', true);

// ---------------------------------------------------------

// Close and output PDF document
// This method has several options, check the source code documentation for more information.
$pdf->Output('example_001.pdf', 'I');

//============================================================+
// END OF FILE
//============================================================+
}