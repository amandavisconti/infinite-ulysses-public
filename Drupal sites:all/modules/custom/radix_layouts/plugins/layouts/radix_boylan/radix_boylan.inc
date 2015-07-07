<?php
// Plugin definition
$plugin = array(
  'title' => t('Radix Boylan'),
  'icon' => 'radix-boylan.png',
  'category' => t('Radix'),
  'theme' => 'radix_boylan',
  'regions' => array(
    'radix-layouts-header' => t('Row 1: First Column'),
    'column1' => t('Row 2: First Column'),
    'column2' => t('Row 2: Second Column'),
    'footercolumn1' => t('Footer First Column'),
    'footercolumn2' => t('Footer Second Column'),
    'footercolumn3' => t('Footer Third Column'),
  ),
);
