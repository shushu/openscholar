
(function ($) {
  Drupal.behaviors.osBoxesAccordion = {
    attach: function (ctx) {
      $('.accordion', ctx).accordion();
    }
  }
})(jQuery);
