<?php
/**
 * @file
 * Template for Radix boylan.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display boylan clearfix <?php if (!empty($classes)) { print $classes; } ?><?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <div class="container-fluid">
    <div class="row">
      <div class="col-md-12 radix-layouts-header panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['radix-layouts-header']; ?>
        </div>
      </div>
    </div>

  <div class="container-fluid">
    <div class="row">
      <div class="col-md-6 radix-layouts-column1 panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['column1']; ?>
        </div>
      </div>
      <div class="col-md-6 radix-layouts-column2 panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['column2']; ?>
        </div>
      </div>
    </div>
    
  <div class="container-fluid">
    <div class="row">
      <div class="col-md-4 radix-layouts-footercolumn1 panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['footercolumn1']; ?>
        </div>
      </div>
      <div class="col-md-4 radix-layouts-footercolumn2 panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['footercolumn2']; ?>
        </div>
      </div>
      <div class="col-md-4 radix-layouts-footercolumn3 panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['footercolumn3']; ?>
        </div>
      </div>
    </div>
  </div>
  
</div><!-- /.boylan -->
