"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function DoctorPage({ params }) {
    const doctorId = params.id;
    const router = useRouter();

    const [doctor, setDoctor] = useState(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");

    const [hospitals, setHospitals] = useState([]);
    const [selectedHospital, setSelectedHospital] = useState(null);

    const [schedule, setSchedule] = useState([]);
    const [loadingSchedule, setLoadingSchedule] = useState(false);

    const [ratings, setRatings] = useState([]);
    const [averageRating, setAverageRating] = useState(null);
    const [loadingRatings, setLoadingRatings] = useState(false);

    const [waitlistCount, setWaitlistCount] = useState(0);
    const [inWaitlist, setInWaitlist] = useState(false);
    const [addingToWaitlist, setAddingToWaitlist] = useState(false);

    const [selectedDate, setSelectedDate] = useState(null);
    const [selectedSlot, setSelectedSlot] = useState(null);
    const [bookingInProgress, setBookingInProgress] = useState(false);
    const [bookingSuccess, setBookingSuccess] = useState("");
    const [bookingError, setBookingError] = useState("");

    const [isLoggedIn, setIsLoggedIn] = useState(false);
    const [userId, setUserId] = useState(null);
    const [userType, setUserType] = useState(null);

    // Состояние для добавления/редактирования отзыва
    const [reviewText, setReviewText] = useState("");
    const [reviewRating, setReviewRating] = useState(5);
    const [isAddingReview, setIsAddingReview] = useState(false);
    const [isEditingReview, setIsEditingReview] = useState(false);
    const [editingReviewId, setEditingReviewId] = useState(null);
    const [reviewError, setReviewError] = useState("");
    const [reviewSuccess, setReviewSuccess] = useState("");
    const [userReview, setUserReview] = useState(null);

    useEffect(() => {
        // Проверка аутентификации
        const loginStatus = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        setIsLoggedIn(loginStatus === 'true');
        setUserId(userData.userId || null);
        setUserType(userData.userType || null);

        // Загрузка информации о враче через API
        fetchDoctorInfo();
    }, [doctorId]);

    const fetchDoctorInfo = async () => {
        try {
            // Получаем данные о враче
            const doctorResponse = await fetch(`https://api.medscheduler.ru/get_doctor_profile?user_id=${doctorId}`);

            if (!doctorResponse.ok) {
                throw new Error("Не удалось получить информацию о враче");
            }

            const doctorData = await doctorResponse.json();

            if (doctorData.success) {
                setDoctor(doctorData.profile);
            } else {
                throw new Error(doctorData.error || "Не удалось получить информацию о враче");
            }

            // Получаем больницы врача
            const hospitalsResponse = await fetch(`https://api.medscheduler.ru/get_doctor_hospitals?doctor_id=${doctorId}`);
            const hospitalsData = await hospitalsResponse.json();

            if (hospitalsResponse.ok && hospitalsData.success) {
                setHospitals(hospitalsData.hospitals);
                if (hospitalsData.hospitals.length > 0) {
                    setSelectedHospital(hospitalsData.hospitals[0]);
                }
            }

            // Загружаем отзывы
            fetchRatings();

            // Получаем количество в листе ожидания
            const waitlistResponse = await fetch(`https://api.medscheduler.ru/get_waitlist_count?doctor_id=${doctorId}`);
            const waitlistData = await waitlistResponse.json();

            if (waitlistResponse.ok && waitlistData.success) {
                setWaitlistCount(waitlistData.count);
            }

            // Проверяем, находится ли пользователь в листе ожидания
            if (isLoggedIn && userId) {
                const userWaitlistResponse = await fetch(`https://api.medscheduler.ru/get_patient_waitlist?patient_id=${userId}`);
                const userWaitlistData = await userWaitlistResponse.json();

                if (userWaitlistResponse.ok && userWaitlistData.success) {
                    // Проверяем, есть ли этот врач в списке ожидания пациента
                    const inWaitlistForThisDoctor = userWaitlistData.waitlist.some(
                        item => item.doctor_id === parseInt(doctorId)
                    );
                    setInWaitlist(inWaitlistForThisDoctor);
                }
            }

        } catch (err) {
            console.error("Error fetching doctor info:", err);
            setError("Не удалось загрузить информацию о враче");
        } finally {
            setLoading(false);
        }
    };

    const fetchRatings = async () => {
        setLoadingRatings(true);
        try {
            // Получаем отзывы
            const ratingsResponse = await fetch(`https://api.medscheduler.ru/get_doctor_feedback_items?doctor_id=${doctorId}`);
            const ratingsData = await ratingsResponse.json();

            if (ratingsResponse.ok && ratingsData.success) {
                setRatings(ratingsData.ratings);

                // Если пользователь авторизован, находим его отзыв
                if (userId) {
                    const userReview = ratingsData.ratings.find(r => r.user_id === userId);
                    if (userReview) {
                        setUserReview(userReview);
                    }
                }
            }

            // Получаем средний рейтинг
            const avgRatingResponse = await fetch(`https://api.medscheduler.ru/get_doctor_feedback_calculated?doctor_id=${doctorId}`);
            const avgRatingData = await avgRatingResponse.json();

            if (avgRatingResponse.ok && avgRatingData.success) {
                setAverageRating(avgRatingData.average);
            }

        } catch (err) {
            console.error("Error fetching ratings:", err);
        } finally {
            setLoadingRatings(false);
        }
    };

    const fetchSchedule = async () => {
        if (!selectedHospital) return;

        setLoadingSchedule(true);
        try {
            const response = await fetch(`https://api.medscheduler.ru/get_doctor_schedule_for_patient?doctor_id=${doctorId}`);
            const data = await response.json();

            if (response.ok && data.success) {
                // Фильтруем расписание по выбранной больнице
                const filteredSchedule = data.schedule.filter(slot =>
                    slot.hospital_id === selectedHospital.hospital_id && slot.slot_status === 'open'
                );

                // Группируем слоты по датам
                const groupedByDate = filteredSchedule.reduce((acc, slot) => {
                    const date = slot.appointment_date;
                    if (!acc[date]) {
                        acc[date] = [];
                    }
                    acc[date].push(slot);
                    return acc;
                }, {});

                setSchedule(groupedByDate);
            } else {
                console.error("Error fetching schedule:", data.error);
            }
        } catch (err) {
            console.error("Error fetching schedule:", err);
        } finally {
            setLoadingSchedule(false);
        }
    };

    useEffect(() => {
        if (selectedHospital) {
            fetchSchedule();
        }
    }, [selectedHospital]);

    const handleHospitalChange = (hospital) => {
        setSelectedHospital(hospital);
        setSelectedDate(null);
        setSelectedSlot(null);
    };

    const handleDateSelect = (date) => {
        setSelectedDate(date);
        setSelectedSlot(null);
    };

    const handleSlotSelect = (slot) => {
        setSelectedSlot(slot);
    };

    const handleBookAppointment = async () => {
        if (!isLoggedIn) {
            router.push('/login');
            return;
        }

        if (!selectedSlot) {
            setBookingError("Выберите время для записи");
            return;
        }

        setBookingInProgress(true);
        setBookingError("");
        setBookingSuccess("");

        try {
            const bookingData = {
                doctor_id: parseInt(doctorId),
                hospital_id: selectedHospital.hospital_id,
                appointment_date: selectedSlot.appointment_date,
                appointment_time: selectedSlot.appointment_time,
                patient_id: parseInt(userId)
            };

            const response = await fetch('https://api.medscheduler.ru/book_appointment', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(bookingData),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setBookingSuccess("Вы успешно записаны на прием");
                setSelectedSlot(null);

                // Обновляем расписание
                fetchSchedule();

                // Сбрасываем сообщение об успехе через 3 секунды
                setTimeout(() => {
                    setBookingSuccess("");
                }, 3000);
            } else {
                setBookingError(data.error || "Ошибка при записи на прием");
            }
        } catch (err) {
            console.error("Error booking appointment:", err);
            setBookingError("Не удалось подключиться к серверу");
        } finally {
            setBookingInProgress(false);
        }
    };

    const handleAddToWaitlist = async () => {
        if (!isLoggedIn) {
            router.push('/login');
            return;
        }

        setAddingToWaitlist(true);
        setError("");

        try {
            const response = await fetch('https://api.medscheduler.ru/add_patient_to_waitlist', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ doctor_id: parseInt(doctorId), patient_id: parseInt(userId) }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setInWaitlist(true);
                setWaitlistCount(waitlistCount + 1);
                alert("Вы успешно добавлены в лист ожидания");
            } else {
                setError(data.error || "Ошибка при добавлении в лист ожидания");
            }
        } catch (err) {
            console.error("Error adding to waitlist:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setAddingToWaitlist(false);
        }
    };

    const handleAddReview = async () => {
        if (!isLoggedIn) {
            router.push('/login');
            return;
        }

        if (!reviewText.trim()) {
            setReviewError("Пожалуйста, напишите текст отзыва");
            return;
        }

        setReviewError("");
        setReviewSuccess("");

        try {
            const response = await fetch('https://api.medscheduler.ru/post_doctor_feedback', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    doctor_ref_id: parseInt(doctorId),
                    user_id: parseInt(userId),
                    text: reviewText,
                    rate: reviewRating,
                    address: selectedHospital ? selectedHospital.full_name : "Онлайн консультация"
                }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setReviewSuccess("Отзыв успешно добавлен");
                setReviewText("");
                setReviewRating(5);
                setIsAddingReview(false);

                // Обновляем отзывы
                fetchRatings();

                // Сбрасываем сообщение об успехе через 3 секунды
                setTimeout(() => {
                    setReviewSuccess("");
                }, 3000);
            } else {
                setReviewError(data.error || "Ошибка при добавлении отзыва");
            }
        } catch (err) {
            console.error("Error adding review:", err);
            setReviewError("Не удалось подключиться к серверу");
        }
    };

    const handleEditReview = async () => {
        if (!userReview) return;

        if (!reviewText.trim()) {
            setReviewError("Пожалуйста, напишите текст отзыва");
            return;
        }

        setReviewError("");
        setReviewSuccess("");

        try {
            const response = await fetch('https://api.medscheduler.ru/edit_doctor_feedback', {
                method: 'PATCH',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    rating_id: userReview.id,
                    user_id: parseInt(userId),
                    rate: reviewRating,
                    text: reviewText,
                    name: userReview.name,
                    date: userReview.date,
                    address: userReview.address
                }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setReviewSuccess("Отзыв успешно обновлен");
                setIsEditingReview(false);

                // Обновляем отзывы
                fetchRatings();

                // Сбрасываем сообщение об успехе через 3 секунды
                setTimeout(() => {
                    setReviewSuccess("");
                }, 3000);
            } else {
                setReviewError(data.error || "Ошибка при обновлении отзыва");
            }
        } catch (err) {
            console.error("Error updating review:", err);
            setReviewError("Не удалось подключиться к серверу");
        }
    };

    const handleDeleteReview = async (reviewId) => {
        if (!confirm("Вы действительно хотите удалить отзыв?")) {
            return;
        }

        try {
            const response = await fetch('https://api.medscheduler.ru/delete_doctor_feedback', {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    rating_id: reviewId,
                    admin_id: parseInt(userId)
                }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                alert("Отзыв успешно удален");

                // Обновляем отзывы
                fetchRatings();
                setUserReview(null);
            } else {
                alert(data.error || "Ошибка при удалении отзыва");
            }
        } catch (err) {
            console.error("Error deleting review:", err);
            alert("Не удалось подключиться к серверу");
        }
    };

    const startAddReview = () => {
        if (!isLoggedIn) {
            router.push('/login');
            return;
        }

        setIsAddingReview(true);
        setReviewText("");
        setReviewRating(5);
        setReviewError("");
        setReviewSuccess("");
    };

    const startEditReview = () => {
        if (!userReview) return;

        setIsEditingReview(true);
        setReviewText(userReview.text);
        setReviewRating(userReview.rate);
        setEditingReviewId(userReview.id);
        setReviewError("");
        setReviewSuccess("");
    };

    // Форматирование даты
    const formatDate = (dateStr) => {
        const date = new Date(dateStr);
        return date.toLocaleDateString('ru-RU', {
            day: '2-digit',
            month: '2-digit',
            year: 'numeric'
        });
    };

    // Форматирование времени
    const formatTime = (timeStr) => {
        return timeStr.substring(0, 5); // Берем только HH:MM из HH:MM:SS
    };

    // Форматирование даты отзыва
    const formatReviewDate = (dateStr) => {
        const date = new Date(dateStr);
        return date.toLocaleDateString('ru-RU', {
            day: '2-digit',
            month: '2-digit',
            year: 'numeric',
            hour: '2-digit',
            minute: '2-digit'
        });
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка информации о враче...</div>
            </div>
        );
    }

    if (error) {
        return (
            <div className="max-w-4xl mx-auto py-8 px-4">
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
                <Link href="/search" className="text-main hover:text-main2 transition-colors">
                    ← Вернуться к поиску
                </Link>
            </div>
        );
    }

    if (!doctor) {
        return (
            <div className="max-w-4xl mx-auto py-8 px-4">
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    Врач не найден
                </div>
                <Link href="/search" className="text-main hover:text-main2 transition-colors">
                    ← Вернуться к поиску
                </Link>
            </div>
        );
    }

    return (
        <div className="max-w-5xl mx-auto py-8 px-4">
            {/* Информация о враче */}
            <div className="bg-white shadow-md rounded-lg p-6 mb-8">
                <div className="flex flex-col md:flex-row md:items-start">
                    <div className="md:w-1/4 flex items-center justify-center md:justify-start mb-4 md:mb-0">
                        {/* Здесь можно добавить фото врача, если оно есть */}
                        <div className="w-32 h-32 bg-gray-200 rounded-full flex items-center justify-center">
                            <span className="text-gray-500">Фото</span>
                        </div>
                    </div>

                    <div className="md:w-3/4">
                        <h1 className="text-2xl md:text-3xl font-bold text-main2">
                            {doctor.last_name} {doctor.first_name} {doctor.patronymic}
                        </h1>

                        <p className="text-lg text-gray-700 mt-2">{doctor.specialty}</p>

                        <div className="flex items-center mt-3">
                            <div className="flex items-center">
                                <svg xmlns="http://www.w3.org/2000/svg" className="h-5 w-5 text-yellow-500" viewBox="0 0 20 20" fill="currentColor">
                                    <path d="M9.049 2.927c.3-.921 1.603-.921 1.902 0l1.07 3.292a1 1 0 00.95.69h3.462c.969 0 1.371 1.24.588 1.81l-2.8 2.034a1 1 0 00-.364 1.118l1.07 3.292c.3.921-.755 1.688-1.54 1.118l-2.8-2.034a1 1 0 00-1.175 0l-2.8 2.034c-.784.57-1.838-.197-1.539-1.118l1.07-3.292a1 1 0 00-.364-1.118L2.98 8.72c-.783-.57-.38-1.81.588-1.81h3.461a1 1 0 00.951-.69l1.07-3.292z" />
                                </svg>
                                <span className="ml-1 text-sm font-medium text-gray-700">
                                    {averageRating ? averageRating.toFixed(1) : "Нет отзывов"}
                                </span>
                                <span className="ml-2 text-sm text-gray-500">
                                    ({ratings.length} {ratings.length === 1 ? "отзыв" :
                                    ratings.length > 1 && ratings.length < 5 ? "отзыва" : "отзывов"})
                                </span>
                            </div>
                        </div>

                        <div className="mt-4 grid grid-cols-1 md:grid-cols-2 gap-3">
                            <div>
                                <h3 className="text-sm font-medium text-gray-500">Стаж работы</h3>
                                <p className="text-base">{doctor.experience} {doctor.experience === 1 ? "год" : doctor.experience < 5 ? "года" : "лет"}</p>
                            </div>
                            <div>
                                <h3 className="text-sm font-medium text-gray-500">Стоимость приема</h3>
                                <p className="text-base text-main font-semibold">{doctor.price} ₽</p>
                            </div>
                            {doctor.education && (
                                <div className="md:col-span-2">
                                    <h3 className="text-sm font-medium text-gray-500">Образование</h3>
                                    <p className="text-base">{doctor.education}</p>
                                </div>
                            )}
                        </div>

                        {hospitals.length > 0 && (
                            <div className="mt-4">
                                <h3 className="text-sm font-medium text-gray-500 mb-2">Клиники</h3>
                                <div className="flex flex-wrap gap-2">
                                    {hospitals.map((hospital) => (
                                        <button
                                            key={hospital.hospital_id}
                                            onClick={() => handleHospitalChange(hospital)}
                                            className={`px-3 py-1 text-sm rounded-full transition-colors ${
                                                selectedHospital && selectedHospital.hospital_id === hospital.hospital_id
                                                ? "bg-main text-white"
                                                : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                            }`}
                                        >
                                            {hospital.full_name}
                                        </button>
                                    ))}
                                </div>
                            </div>
                        )}
                    </div>
                </div>
            </div>

            {/* Табы для расписания и отзывов */}
            <div className="bg-white shadow-md rounded-lg overflow-hidden mb-8">
                <div className="border-b border-gray-200">
                    <nav className="flex -mb-px">
                        <button
                            onClick={() => document.getElementById('schedule').scrollIntoView({ behavior: 'smooth' })}
                            className="py-4 px-6 text-center border-b-2 border-main text-main font-medium text-sm"
                        >
                            Расписание и запись
                        </button>
                        <button
                            onClick={() => document.getElementById('reviews').scrollIntoView({ behavior: 'smooth' })}
                            className="py-4 px-6 text-center border-b-2 border-transparent hover:text-main hover:border-gray-300 font-medium text-sm text-gray-500"
                        >
                            Отзывы ({ratings.length})
                        </button>
                    </nav>
                </div>

                {/* Расписание */}
                <div id="schedule" className="p-6">
                    <h2 className="text-xl font-semibold text-main2 mb-4">Расписание и запись на прием</h2>

                    {!selectedHospital ? (
                        <div className="text-center py-6">
                            <p className="text-gray-600 mb-4">Выберите клинику для просмотра расписания</p>
                        </div>
                    ) : loadingSchedule ? (
                        <div className="text-center py-6">
                            <p className="text-gray-600">Загрузка расписания...</p>
                        </div>
                    ) : Object.keys(schedule).length === 0 ? (
                        <div className="bg-gray-50 rounded-lg p-6 text-center">
                            <p className="text-gray-600 mb-4">Нет доступных слотов для записи</p>

                            {!inWaitlist && (
                                <div>
                                    <p className="text-gray-500 mb-3">
                                        Вы можете добавить себя в лист ожидания. В случае отмены записи другим пациентом,
                                        вы получите уведомление о возможности записи.
                                    </p>
                                    <p className="text-gray-500 mb-4">
                                        Текущее количество в листе ожидания: <span className="font-semibold">{waitlistCount}</span>
                                    </p>
                                    <button
                                        onClick={handleAddToWaitlist}
                                        disabled={addingToWaitlist || !isLoggedIn}
                                        className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors disabled:opacity-50"
                                    >
                                        {addingToWaitlist ? "Добавление..." : "Добавить в лист ожидания"}
                                    </button>
                                </div>
                            )}

                            {inWaitlist && (
                                <p className="text-main font-medium">
                                    Вы уже добавлены в лист ожидания
                                </p>
                            )}
                        </div>
                    ) : (
                        <div>
                            {/* Выбор даты */}
                            <div className="mb-6">
                                <h3 className="text-lg font-medium text-gray-700 mb-3">Выберите дату</h3>
                                <div className="flex flex-wrap gap-2">
                                    {Object.keys(schedule).map((date) => (
                                        <button
                                            key={date}
                                            onClick={() => handleDateSelect(date)}
                                            className={`px-4 py-2 rounded-md transition-colors ${
                                                selectedDate === date
                                                ? "bg-main text-white"
                                                : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                            }`}
                                        >
                                            {formatDate(date)}
                                        </button>
                                    ))}
                                </div>
                            </div>

                            {/* Выбор времени */}
                            {selectedDate && (
                                <div className="mb-6">
                                    <h3 className="text-lg font-medium text-gray-700 mb-3">Выберите время</h3>
                                    <div className="grid grid-cols-4 sm:grid-cols-6 md:grid-cols-8 gap-2">
                                        {schedule[selectedDate].map((slot) => (
                                            <button
                                                key={`${slot.appointment_date}-${slot.appointment_time}`}
                                                onClick={() => handleSlotSelect(slot)}
                                                className={`px-3 py-2 rounded-md text-center transition-colors ${
                                                    selectedSlot && 
                                                    selectedSlot.appointment_date === slot.appointment_date && 
                                                    selectedSlot.appointment_time === slot.appointment_time
                                                    ? "bg-main text-white"
                                                    : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                                }`}
                                            >
                                                {formatTime(slot.appointment_time)}
                                            </button>
                                        ))}
                                    </div>
                                </div>
                            )}

                            {/* Подтверждение записи */}
                            {selectedSlot && (
                                <div className="mt-6 p-4 bg-gray-50 rounded-lg">
                                    <h3 className="text-lg font-semibold text-gray-800 mb-2">Подтверждение записи</h3>

                                    <div className="grid grid-cols-1 md:grid-cols-2 gap-3 mb-4">
                                        <div>
                                            <p className="text-sm text-gray-500">Врач</p>
                                            <p className="text-base">{doctor.last_name} {doctor.first_name} {doctor.patronymic}</p>
                                        </div>
                                        <div>
                                            <p className="text-sm text-gray-500">Специальность</p>
                                            <p className="text-base">{doctor.specialty}</p>
                                        </div>
                                        <div>
                                            <p className="text-sm text-gray-500">Клиника</p>
                                            <p className="text-base">{selectedHospital.full_name}</p>
                                        </div>
                                        <div>
                                            <p className="text-sm text-gray-500">Дата и время</p>
                                            <p className="text-base">
                                                {formatDate(selectedSlot.appointment_date)} в {formatTime(selectedSlot.appointment_time)}
                                            </p>
                                        </div>
                                        <div>
                                            <p className="text-sm text-gray-500">Стоимость</p>
                                            <p className="text-base text-main font-semibold">{doctor.price} ₽</p>
                                        </div>
                                    </div>

                                    {bookingError && (
                                        <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                                            {bookingError}
                                        </div>
                                    )}

                                    {bookingSuccess && (
                                        <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                                            {bookingSuccess}
                                        </div>
                                    )}

                                    <div className="flex gap-3">
                                        <button
                                            onClick={handleBookAppointment}
                                            disabled={bookingInProgress || !isLoggedIn}
                                            className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors disabled:opacity-50"
                                        >
                                            {bookingInProgress ? "Бронирование..." : "Записаться на прием"}
                                        </button>
                                        <button
                                            onClick={() => setSelectedSlot(null)}
                                            className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                                        >
                                            Отмена
                                        </button>
                                    </div>

                                    {!isLoggedIn && (
                                        <p className="mt-3 text-sm text-gray-600">
                                            Для записи на прием необходимо <Link href="/login" className="text-main hover:text-main2">войти в аккаунт</Link>
                                        </p>
                                    )}
                                </div>
                            )}
                        </div>
                    )}
                </div>

                {/* Отзывы */}
                <div id="reviews" className="p-6 border-t border-gray-200">
                    <div className="flex justify-between items-center mb-6">
                        <h2 className="text-xl font-semibold text-main2">Отзывы о враче</h2>

                        {isLoggedIn && userType === 'patient' && !userReview && !isAddingReview && (
                            <button
                                onClick={startAddReview}
                                className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                            >
                                Оставить отзыв
                            </button>
                        )}

                        {isLoggedIn && userType === 'patient' && userReview && !isEditingReview && (
                            <div className="flex gap-2">
                                <button
                                    onClick={startEditReview}
                                    className="px-4 py-2 bg-gray-100 text-gray-700 rounded hover:bg-gray-200 transition-colors"
                                >
                                    Редактировать отзыв
                                </button>
                                <button
                                    onClick={() => handleDeleteReview(userReview.id)}
                                    className="px-4 py-2 bg-red-100 text-red-700 rounded hover:bg-red-200 transition-colors"
                                >
                                    Удалить
                                </button>
                            </div>
                        )}
                    </div>

                    {(isAddingReview || isEditingReview) && (
                        <div className="bg-gray-50 rounded-lg p-6 mb-6">
                            <h3 className="text-lg font-medium text-gray-800 mb-4">
                                {isAddingReview ? "Новый отзыв" : "Редактирование отзыва"}
                            </h3>

                            {reviewError && (
                                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                                    {reviewError}
                                </div>
                            )}

                            {reviewSuccess && (
                                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                                    {reviewSuccess}
                                </div>
                            )}

                            <div className="mb-4">
                                <label className="block text-sm font-medium text-gray-700 mb-2">
                                    Ваша оценка
                                </label>
                                <div className="flex gap-2">
                                    {[1, 2, 3, 4, 5].map((rating) => (
                                        <button
                                            key={rating}
                                            type="button"
                                            onClick={() => setReviewRating(rating)}
                                            className={`w-10 h-10 rounded-full flex items-center justify-center transition-colors ${
                                                reviewRating >= rating ? "bg-yellow-400 text-white" : "bg-gray-200 text-gray-700"
                                            }`}
                                        >
                                            {rating}
                                        </button>
                                    ))}
                                </div>
                            </div>

                            <div className="mb-4">
                                <label htmlFor="reviewText" className="block text-sm font-medium text-gray-700 mb-2">
                                    Ваш отзыв
                                </label>
                                <textarea
                                    id="reviewText"
                                    rows="4"
                                    value={reviewText}
                                    onChange={(e) => setReviewText(e.target.value)}
                                    placeholder="Расскажите о вашем опыте посещения этого врача"
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                />
                            </div>

                            <div className="flex gap-3">
                                <button
                                    onClick={isAddingReview ? handleAddReview : handleEditReview}
                                    className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                                >
                                    {isAddingReview ? "Отправить отзыв" : "Сохранить изменения"}
                                </button>
                                <button
                                    onClick={() => isAddingReview ? setIsAddingReview(false) : setIsEditingReview(false)}
                                    className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                                >
                                    Отмена
                                </button>
                            </div>
                        </div>
                    )}

                    {loadingRatings ? (
                        <div className="text-center py-6">
                            <p className="text-gray-600">Загрузка отзывов...</p>
                        </div>
                    ) : ratings.length === 0 ? (
                        <div className="text-center py-6 bg-gray-50 rounded-lg">
                            <p className="text-gray-600 mb-2">Нет отзывов</p>
                            <p className="text-gray-500 text-sm">Будьте первым, кто оставит отзыв об этом враче</p>
                        </div>
                    ) : (
                        <div className="space-y-6">
                            {ratings.map((review) => (
                                <div key={review.id} className="p-4 border rounded-lg">
                                    <div className="flex justify-between">
                                        <div>
                                            <div className="flex items-center">
                                                <span className="font-medium text-gray-800">{review.name}</span>
                                                <span className="mx-2 text-gray-300">|</span>
                                                <div className="flex">
                                                    {Array.from({ length: 5 }).map((_, i) => (
                                                        <svg
                                                            key={i}
                                                            xmlns="http://www.w3.org/2000/svg"
                                                            className={`h-4 w-4 ${
                                                                i < review.rate ? "text-yellow-400" : "text-gray-300"
                                                            }`}
                                                            viewBox="0 0 20 20"
                                                            fill="currentColor"
                                                        >
                                                            <path d="M9.049 2.927c.3-.921 1.603-.921 1.902 0l1.07 3.292a1 1 0 00.95.69h3.462c.969 0 1.371 1.24.588 1.81l-2.8 2.034a1 1 0 00-.364 1.118l1.07 3.292c.3.921-.755 1.688-1.54 1.118l-2.8-2.034a1 1 0 00-1.175 0l-2.8 2.034c-.784.57-1.838-.197-1.539-1.118l1.07-3.292a1 1 0 00-.364-1.118L2.98 8.72c-.783-.57-.38-1.81.588-1.81h3.461a1 1 0 00.951-.69l1.07-3.292z" />
                                                        </svg>
                                                    ))}
                                                </div>
                                            </div>
                                            <p className="text-sm text-gray-500 mt-1">
                                                {formatReviewDate(review.date)}
                                            </p>
                                        </div>
                                        <div className="text-sm text-gray-500">
                                            {review.address}
                                        </div>
                                    </div>
                                    <p className="mt-3 text-gray-700">{review.text}</p>

                                    {userType === 'patient' && review.user_id === userId && (
                                        <div className="mt-3 flex gap-2 justify-end">
                                            <button
                                                onClick={startEditReview}
                                                className="text-sm text-main hover:text-main2"
                                            >
                                                Редактировать
                                            </button>
                                            <button
                                                onClick={() => handleDeleteReview(review.id)}
                                                className="text-sm text-red-600 hover:text-red-800"
                                            >
                                                Удалить
                                            </button>
                                        </div>
                                    )}
                                </div>
                            ))}
                        </div>
                    )}
                </div>
            </div>

            <div className="mt-6">
                <Link href="/search" className="text-main hover:text-main2 transition-colors">
                    ← Вернуться к поиску врачей
                </Link>
            </div>
        </div>
    );
}
