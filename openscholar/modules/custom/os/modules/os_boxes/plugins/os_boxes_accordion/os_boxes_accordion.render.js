
(function ($) {
  Drupal.behaviors.osBoxesAccordion = {
    attach: function (ctx) {
      $('.accordion', ctx).accordion({
        collapsible: true
      });
    }
  }
})(jQuery);
