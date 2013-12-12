/**
 * jQuery to toggle form elements according to content type
 */

(function($) {
  Drupal.behaviors.os_sv_list = {
    attach : function(context) {
      $('#os_sv_list_content_type').once('once', function() {
        // when content type changes, update sorting options list.
        $('#os_sv_list_content_type').change(function() {
          var $sortby = $('#edit-sort-by');
          var $display_style = $('#edit-display');
          var $vocabs = $('#edit-vocabs');
  
          var content_type = $('#os_sv_list_content_type').val();
          var more_link = $('#more_link_div input[name="more_link"]');
          var defaults = Drupal.settings.more_link_defaults;
  
          
          //apply content_type appropriate sorts when ct changes
          $sortby.children('option').each(function() {
            var sorts = Drupal.settings.sv_list_sort;
            
            var this_sort = $(this).attr('value');
            var hide = ((sorts[this_sort] !== undefined) && ($.inArray(content_type, sorts[this_sort]['bundle']) == -1));            
            $(this).attr('hidden', hide).attr('disabled', hide);
          });
          
          //uncheck if selected option is no longer valid.
          $sortby.children('option:checked').filter(':disabled').attr('selected', false);
  
          
          //apply content_type appropriate sorts when ct changes
          $display_style.children('option').each(function() {
            var this_display = $(this).attr('value');
            var hide = ($.inArray(this_display, Drupal.settings.entity_view_modes[content_type]) == -1)
            $(this).attr('hidden', hide).attr('disabled', hide);
          });
          
          //uncheck if selected option is no longer valid.
          $display_style.children('option:checked').filter(':disabled').attr('selected', false);

          // swap out the more link url.
          more_link.val(defaults[content_type]);
          
          
          
          //apply content type to available vocabs
          var hidden = true;
          for (var vid in Drupal.settings.sv_list_vocab_bundles) {
            var $div = $vocabs.find('.form-item-vocabs-vocab-' + vid);
            if ((content_type == 'all') || $.inArray(content_type, Drupal.settings.sv_list_vocab_bundles[vid]) != -1) {
              $div.show();
              hidden = false
            } else {
              $div.hide();
            }
          }

          //show/hide the vocab label if there are any remaining vocabs
          if (hidden) {
            $vocabs.hide();
          } else {
            $vocabs.show();
          }
        });
  
        // perform the change callback once now.
        $('#os_sv_list_content_type').change();
  
        // Get the default value of the content_type.
        var content_type = $('#os_sv_list_content_type').val();
        var show_all_checked = $('#biblio_show_all_check').is(':checked') ? true : false;
  
        });

      // Select2.
      $("#vocabs", context).addClass('select2-processed').find('.form-select:not(.select2-processed)').select2({
          placeholder: Drupal.t("Click here to select terms")
      });
    }
  };
}(jQuery));
