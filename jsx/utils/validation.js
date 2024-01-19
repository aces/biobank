706   /**                                                                           
707    * Validate a pool of speciments                                              
708    *                                                                            
709    * @param {object} pool - The pool to validate                                
710    *                                                                            
711    * @return {object} an object of any errors                                   
712    */                                                                           
713   validatePool(pool) {                                                          
714     let regex;                                                                  
715     const errors = {};                                                          
716                                                                                 
717     const required = ['label', 'quantity', 'unitId', 'date', 'time'];           
718                                                                                 
719     required.forEach((field) => {                                               
720       if (!pool[field]) {                                                       
721         errors[field] = 'This field is required! ';                             
722       }                                                                         
723     });                                                                         
724                                                                                 
725     if (isNaN(parseInt(pool.quantity)) || !isFinite(pool.quantity)) {           
726       errors.quantity = 'This field must be a number! ';                        
727     }                                                                           
728                                                                                 
729     // validate date                                                            
730     regex = /^[12]\d{3}\-(0[1-9]|1[012])\-(0[1-9]|[12][0-9]|3[01])$/;           
731     if (regex.test(pool.date) === false ) {                                     
732       errors.date = 'This field must be a valid date! ';                        
733     }                                                                           
734                                                                                 
735     // validate time                                                            
736     regex = /^([01]\d|2[0-3]):([0-5]\d)$/;                                      
737     if (regex.test(pool.time) === false) {                                      
738       errors.time = 'This field must be a valid time! ';                        
739     }                                                                           
740                                                                                 
741     if (pool.specimenIds == null || pool.specimenIds.length < 2) {              
742       errors.total = 'Pooling requires at least 2 specimens';                   
743     };                                                                          
744                                                                                 
745     return errors;                                                              
746   }     

661   /**                                                                           
662    * Validate a container object                                                
663    *                                                                            
664    * @param {object} container - the container to validate                      
665    * @param {string} key - unused?                                              
666    *                                                                            
667    * @return {object} - an object full of errors                                
668    */                                                                           
669    validateContainer(container) {                                               
670        const errors = {};                                                       
671                                                                                 
672        const required = [                                                       
673          'barcode',                                                             
674          'typeId',                                                              
675          'temperature',                                                         
676          'statusId',                                                            
677          'centerId',                                                            
678        ];                                                                       
679                                                                                 
680        const float = [                                                          
681          'temperature',                                                         
682        ];                                                                       
683                                                                                 
684        required.forEach((field) => {                                            
685            if (!container[field]?.trim()) {                                     
686                errors[field] = 'This field is required!';                       
687            }                                                                    
688        });                                                                      
689                                                                                 
690        float.forEach((field) => {                                               
691            const value = parseFloat(container[field]);                          
692            if (isNaN(value) || value === parseInt(value)) {                     
693                errors[field] = 'This field must be a floating-point number!';   
694            }                                                                    
695        });                                                                      
696                                                                                 
697        const barcodeSet = new Set(this.state.data.containers.map(c => c.id !== container.id && c.barcode));
698        if (barcodeSet.has(container.barcode)) {                                 
699            errors.barcode = 'Barcode must be unique.';                          
700        }                                                                        
701                                                                                 
702        // TODO: Regex barcode check to be added later                           
703                                                                                 
704        return errors;                                                           
705    }  

563    * Validate a process                                                         
564    *                                                                            
565    * @param {object} process - the process to validate                          
566    * @param {object} attributes - the attributes of the process                 
567    * @param {array} required - the required fields                              
568    * @param {array} number - an array of fields that must be numbers            
569    *                                                                            
570    * @return {object} errors                                                    
571    */                                                                           
572   validateProcess(process, attributes, required, number) {                      
573     let errors = {};                                                            
574     let regex;                                                                  
575                                                                                 
576     // validate required fields                                                 
577     required && required.map((field) => {                                       
578       if (!process[field]) {                                                    
579         errors[field] = 'This field is required! ';                             
580       }                                                                         
581     });                                                                         
582                                                                                 
583     // validate floats                                                          
584     number && number.map((field) => {                                           
585       if (isNaN(parseInt(process[field])) || !isFinite(process[field])) {       
586         errors[field] = 'This field must be a number! ';                        
587       }                                                                         
588     });                                                                         
589                                                                                 
590     // validate date                                                            
591     regex = /^[12]\d{3}\-(0[1-9]|1[012])\-(0[1-9]|[12][0-9]|3[01])$/;           
592     if (regex.test(process.date) === false ) {                                  
593       errors.date = 'This field must be a valid date! ';                        
594     }                                                                           
595                                                                                 
596     // validate time                                                            
597     regex = /^([01]\d|2[0-3]):([0-5]\d)$/;                                      
598     if (regex.test(process.time) === false) {                                   
599       errors.time = 'This field must be a valid time! ';                        
600     }                                                                           
601                                                                                 
602     // validate custom attributes                                               
603     if (!isEmpty(process.data)) {                                               
604       errors.data = {};                                                         
605       const specimenopts = this.state.options.specimen;                         
606       const datatypes = specimenopts.attributeDatatypes;                        
607                                                                                 
608       const protocolId = process.protocolId;                                    
609       const protocolAttributes = specimenopts.protocolAttributes[protocolId];   
610       // FIXME: This if statement was introduced because certain processes have 
611       // a data object even though their protocol isn't associated with attributes.
612       // This is a sign of bad importing/configuration and should be fixed in configuration
613       // rather than here.                                                      
614       if (protocolAttributes) {                                                 
615         Object.keys(protocolAttributes)                                         
616           .forEach((attributeId) => {                                           
617           // validate required                                                  
618           if (protocolAttributes[attributeId].required == 1                     
619               && !process.data[attributeId]) {                                  
620             errors.data[attributeId] = 'This field is required!';               
621           }                                                                     
622                                                                                 
623           const dataTypeId= attributes[attributeId].datatypeId;                 
624           // validate number                                                    
625           if (datatypes[dataTypeId].datatype === 'number') {                    
626             if (isNaN(parseInt(process.data[attributeId])) ||                   
627                 !isFinite(process.data[attributeId])) {                         
628               errors.data[attributeId] = 'This field must be a number!';        
629             }                                                                   
630           }                                                                     
631                                                                                 
632           // validate date                                                      
633           if (datatypes[dataTypeId].datatype === 'date') {                      
634             regex = /^[12]\d{3}\-(0[1-9]|1[012])\-(0[1-9]|[12][0-9]|3[01])$/;   
635             if (regex.test(process.data[attributeId]) === false ) {             
636               errors.data[attributeId] = 'This field must be a valid date! ';   
637             }                                                                   
638           }                                                                     
639                                                                                 
640           // validate time                                                      
641           if (datatypes[dataTypeId].datatype === 'time') {                      
642             regex = /^([01]\d|2[0-3]):([0-5]\d)$/;                              
643             if (regex.test(process.data[attributeId]) === false) {              
644               errors.data[attributeId] = 'This field must be a valid time! ';   
645             }                                                                   
646           }                                                                     
647                                                                                 
648           // TODO: Eventually introduce file validation.                        
649         });                                                                     
650       }                                                                         
651                                                                                 
652       if (isEmpty(errors.data)) {                                               
653         delete errors.data;                                                     
654       }                                                                         
655     }                                                                           
656                                                                                 
657     // Return Errors                                                            
658     return errors;                                                              
659   }      

460   /**                                                                           
461    * Validate a specimen                                                        
462    *                                                                            
463    * @param {object} specimen - the specimen to validate                        
464    * @param {string} key - unused?                                              
465    *                                                                            
466    * @return {object} an object of errors                                       
467    */                                                                           
468   validateSpecimen(specimen, key) {                                             
469     const errors = {};                                                          
470                                                                                 
471     const required = [                                                          
472       'typeId',                                                                 
473       'quantity',                                                               
474       'unitId',                                                                 
475       'candidateId',                                                            
476       'sessionId',                                                              
477       'projectIds',                                                             
478       'collection',                                                             
479     ];                                                                          
480     const float = ['quantity'];                                                 
481     const positive = ['quantity', 'fTCycle'];                                   
482     const integer = ['fTCycle'];                                                
483                                                                                 
484     required.map((field) => {                                                   
485       // TODO: seems like for certain cases it needs to be !== null             
486       if (!specimen[field]) {                                                   
487         errors[field] = 'This field is required! ';                             
488       }                                                                         
489     });                                                                         
490                                                                                 
491     float.map((field) => {                                                      
492       if (isNaN(parseInt(specimen[field])) || !isFinite(specimen[field])) {     
493         errors[field] = 'This field must be a number! ';                        
494       }                                                                         
495     });                                                                         
496                                                                                 
497     positive.map((field) => {                                                   
498       if (specimen[field] != null && specimen[field] < 0) {                     
499         errors[field] = 'This field must not be negative!';                     
500       }                                                                         
501     });                                                                         
502                                                                                 
503     integer.map((field) => {                                                    
504       if (specimen[field] != null                                               
505           && !/^\+?(0|[1-9]\d*)$/.test(specimen[field])                         
506       ) {                                                                       
507         errors[field] = 'This field must be an integer!';                       
508       }                                                                         
509     });                                                                         
510                                                                                 
511     const optspecimen = this.state.options.specimen;                            
512     errors.collection =                                                         
513       this.validateProcess(                                                     
514         specimen.collection,                                                    
515         optspecimen.protocolAttributes[specimen.collection.protocolId],         
516         [                                                                       
517           'protocolId',                                                         
518           'examinerId',                                                         
519           'quantity',                                                           
520           'unitId',                                                             
521           'centerId',                                                           
522           'date',                                                               
523           'time',                                                               
524         ],                                                                      
525         ['quantity']                                                            
526       );                                                                        
527                                                                                 
528     // collection should only be set if there are errors associated with it.    
529     if (isEmpty(errors.collection)) {                                           
530       delete errors.collection;                                                 
531     }                                                                           
532                                                                                 
533     if (specimen.preparation) {                                                 
534       errors.preparation =                                                      
535         this.validateProcess(                                                   
536           specimen.preparation,                                                 
537           optspecimen.protocolAttributes[specimen.preparation.protocolId],      
538           ['protocolId', 'examinerId', 'centerId', 'date', 'time']              
539         );                                                                      
540     }                                                                           
541                                                                                 
542     if (isEmpty(errors.preparation)) {                                          
543       delete errors.preparation;                                                
544     }                                                                           
545                                                                                 
546     if (specimen.analysis) {                                                    
547       errors.analysis =                                                         
548         this.validateProcess(                                                   
549           specimen.analysis,                                                    
550           optspecimen.protocolAttributes[specimen.analysis.protocolId],         
551           ['protocolId', 'examinerId', 'centerId', 'date', 'time']              
552         );                                                                      
553     }                                                                           
554                                                                                 
555     if (isEmpty(errors.analysis)) {                                             
556       delete errors.analysis;                                                   
557     }                                                                           
558                                                                                 
559     return errors;                                                              
560   }      
