/*jslint browser: true, white: false, onevar: false */
/*global jQuery,$,window,net */
jQuery(function(jq) {
    // Handle hiding/showing maintainers
    /* #463 - commented out until this bug is fixed
    jq('#maintainers').truncate( 50, {
        chars: /\s/,
        trail: [ " <a href='#' class='truncate_show'>&#187;</a>", " <a href='#' class='truncate_hide'>&#171;</a>" ]
    });
    */

    // Handle hiding/showing maintainers if more than five
    var moreMaintainers = document.getElementById('moreMaintainers');
    if (moreMaintainers) {
        // #337 - IE isn't properly handling jQuery's toggle() method on the
        // span. Firefox knows that it should be display: inline, but not IE.
        // So we'll just manually toggle the CSS and call it good.
        jq(moreMaintainers).css('display','none');
        var moreLink = jq('<a href="#">&#8230;</a>').click(function(e) {
            e.preventDefault();
            jq(this).remove();
            jq(moreMaintainers).css('display','inline');
        });
        jq('#maintainers').append(moreLink);
    }
        
    // Handle hiding/showing reviews
    jq('.review-txt').truncate( 250, {
        trail: [ " (<a href='#' class='truncate_show'>more</a>&#8230;)", " (&#8230;<a href='#' class='truncate_hide'>less</a>)" ]
    });
    
    jq('#related p').truncate( 100, {
        chars: /\s/,
        trail: [ " <a href='#' class='truncate_show'>&#8230;</a>", " <a href='#' class='truncate_hide'>&#171;</a>" ]
    });
    
    // Generic method used by any box that has a filter
    var filter_change = function(e) {
        var id = e.data.id,
            url = e.data.url,
            selector = e.data.selector,
            params = e.data.params,
            selected = jq(id + ' form select option:selected').val(),
            list = jq(id + ' .list'),
            indicator = jq(new Image()),
            reveal = function() {
                indicator.fadeOut(function() {
                    indicator.remove();
                    list.slideDown('fast');
                });
            };
        
        indicator.load(function() {
            jq(this).hide();
            list.after(this);
            jq(this).fadeIn();
        }).attr('src', net.sf.cdn_url + '/img/indicator.gif').css("padding-top","0.8333em");
        
        list.slideUp('fast', function() {
            var suffix = [
                    '?',
                    selected !== 'everything' ? 'filter=' + selected : '',
                    params,
                    selector ? ' ' + selector : ''
                ],
                url_suffix = suffix.join(''); 
            list.load(url + url_suffix, reveal);
        });
    };
    
    // Setup the feed filter
    jq('#feed-filter input[type="submit"]').remove();
    jq('#feed-filter select').bind('change', { id: '#feed', url: 'feed' }, filter_change);
    
    // Setup the review filter
    jq('#review-filter input[type="submit"]').remove();
    jq('#review-filter select').bind('change', { id: '#reviews-n-ratings', url: 'project_reviews', params: '&max=3'}, filter_change);
   
    // Load the review form, if required
    if (jq('#review-form').length) {
        jq('#review-form').reviewform();
    }
    
    // Setup screenshots modal
    var bf_display = ( $('#screenshots li').length > 5 ) ? true : false ;
    jq('#screenshots .ad-gallery').adGallery({
        loader_image: net.sf.cdn_url + '/img/gallery/loader.gif',
        slideshow: { enable: false },
        cycle: false,
        display_back_and_forward: bf_display
    });

    // auto-open dialog if the URL contains the hash for a particular screenshot
    jq('#screenshots').dialog({
        autoOpen: (window.location.hash && window.location.hash.indexOf('#ad-image') === 0),
        bgiframe: true,
        modal: true,
        width: 634
    });

    // Click to open screenshots modal
    jq('div.b-proj__picrel a').click(function(e){
        e.preventDefault();
        jq('#screenshots').dialog('open').parent().css('left', (($(window).width() - $('#doc4').width()) / 2) );
    });

    // Hide/show project details
    function hide_details_fn(e) {
        e.preventDefault();
        jq(this).text('Show project details');
        jq('#project-details').hide();
        jq.cookie('project_details', null);
    }
    function show_details_fn(e) {
        e.preventDefault();
        jq(this).text('Hide project details');
        jq('#project-details').show();
        jq.cookie('project_details', 'show');
    }
    if (!jq.cookie('project_details')) {
        jq('#project-details').hide().after('<div id="project-reveal"><a href="#more">Show project details</a></div>');
    } else {
        jq('#project-details').after('<div id="project-reveal"><a href="#more">Hide project details</a></div>');
    }
    jq('#project-reveal a').toggle(show_details_fn, hide_details_fn);

    // Review pagination
    function load_more_reviews(e) {
        e.preventDefault();
        
        var link = this,
            reviews_box = jq(link).parent(),
            reviews_list = jq('ul.list', reviews_box);
            
        jq.get(this.href, {}, function(result) {
            var new_reviews = jq('.project-review', result),
                more_reviews = jq('#more-reviews', result);
                
            new_reviews.removeClass('first');
            reviews_list.append(new_reviews);
            reviews_box.append(more_reviews);
            more_reviews.click(load_more_reviews);
            jq(link).remove();
        });
    }
    jq('#more-reviews').click(load_more_reviews);
    
    // Initialize editing of features and icons
    if (net.sf.is_admin) {
        if (jq('ul.features').length) {
            jq('#edit-features').remove();
        } else {
            jq('#edit-features').replaceWith('<ul class="features"></ul>');
        }
        jq('.features').listedit({
            action: 'update',
            add_value: "Add a new feature",
            html: [ net.sf.antixsrf ]
        });
        var icon_form = jq('#icon-dialog form');
        icon_form.validate({
            rules: {
                icon: {
                    required: true,
                    accept: "png|gif|jpeg|jpg"
                }
            }
        });
        jq('#icon-dialog').dialog({
            bgiframe: true,
            autoOpen: false,
            modal: true,
            width: 300,
            buttons: {
                'Save icon': function() {
                    icon_form.submit();
                },
                'Cancel': function() {
                    $(this).dialog('close');
                }
            }
        });
        $('#edit-icon').click(function(e) {
            e.preventDefault();
            $('#icon-dialog').dialog('open');
        });
    }
    
    // Load up the full project nav if logged in, per SFPY-223
    if (jq('#nav-project').children().length === 0) {
        var nav = jq('#nav-project');
        nav.load(nav.metadata().url + ' .nav', function() {
            jq('#nav-project ul.nav > li:last-child').addClass('last');
            jq('#nav-project ul.nav > li').each(function() {
                jq('li[class!=disabled]', this).eq(0).addClass('first');
            });
            jq('#nav-project ul.nav').addClass('b-hornav').removeClass('nav').droppy({speed: 0, delay: 250});
            jq('#menu_consume_' + document.body.id).addClass('selected');
        });
    }
});

